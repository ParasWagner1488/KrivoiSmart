import hashlib
import json
import time
from typing import List, Dict, Optional, Tuple, Any
from dataclasses import dataclass, asdict
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import ed25519
from concurrent.futures import ThreadPoolExecutor
import aiohttp
import asyncio
from fastapi import FastAPI, HTTPException
import uvicorn
from pydantic import BaseModel


@dataclass
class Block:
    index: int
    timestamp: float
    transactions: List['Transaction']
    proof: str
    previous_hash: str
    validator: str
    
    def hash(self) -> str:
        block_string = json.dumps(asdict(self), sort_keys=True).encode()
        return hashlib.sha256(block_string).hexdigest()

@dataclass
class Transaction:
    sender: str
    recipient: str
    amount: float
    fee: float
    signature: str
    contract_call: Optional['ContractCall'] = None
    
    def validate(self) -> bool:
        if self.sender == "0":
            return True
            
        try:
            public_key = ed25519.Ed25519PublicKey.from_public_bytes(
                bytes.fromhex(self.sender)
            )
            message = f"{self.sender}{self.recipient}{self.amount}{self.fee}".encode()
            if self.contract_call:
                message += json.dumps(asdict(self.contract_call)).encode()
                
            public_key.verify(
                bytes.fromhex(self.signature),
                message
            )
            return True
        except:
            return False

@dataclass
class ContractCall:
    contract_address: str
    method: str
    args: List[Any]
    value: float = 0  # Количество токенов для передачи контракту


class SmartContract:
    """Базовый класс для смарт-контрактов"""
    
    def __init__(self, address: str, owner: str):
        self.address = address
        self.owner = owner
        self.storage: Dict[str, Any] = {}
        self.balance = 0.0
        
    def execute(self, method: str, args: List[Any], sender: str, value: float) -> Any:
        self.balance += value
        if hasattr(self, method):
            return getattr(self, method)(*args, sender=sender)
        raise ValueError(f"Method {method} not found")

class TokenContract(SmartContract):
    
    def __init__(self, address: str, owner: str):
        super().__init__(address, owner)
        self.storage['balances'] = {owner: 1000000.0}
        self.storage['allowed'] = {}
        
    def transfer(self, to: str, amount: float, sender: str) -> bool:
        if self.storage['balances'].get(sender, 0) >= amount:
            self.storage['balances'][sender] = self.storage['balances'].get(sender, 0) - amount
            self.storage['balances'][to] = self.storage['balances'].get(to, 0) + amount
            return True
        return False
        
    def approve(self, spender: str, amount: float, sender: str) -> bool:
        self.storage['allowed'][(sender, spender)] = amount
        return True
        
    def transfer_from(self, from_addr: str, to: str, amount: float, sender: str) -> bool:
        allowed = self.storage['allowed'].get((from_addr, sender), 0)
        if allowed >= amount and self.storage['balances'].get(from_addr, 0) >= amount:
            self.storage['balances'][from_addr] -= amount
            self.storage['balances'][to] += amount
            self.storage['allowed'][(from_addr, sender)] -= amount
            return True
        return False


class Blockchain:
    def __init__(self):
        self.chain: List[Block] = []
        self.current_transactions: List[Transaction] = []
        self.contracts: Dict[str, SmartContract] = {}
        self.stakes: Dict[str, float] = {}
        self.node_address: str = self.generate_address()
        self.peers: set = set()
        
        self.new_block(previous_hash="1", proof="0")
        
        self.deploy_contract(TokenContract, "0", "0")
    
    def generate_address(self) -> str:
        private_key = ed25519.Ed25519PrivateKey.generate()
        public_key = private_key.public_key()
        return public_key.public_bytes(
            encoding=serialization.Encoding.Raw,
            format=serialization.PublicFormat.Raw,
        ).hex()
    
    def new_block(self, proof: str, previous_hash: str = None) -> Block:
        block = Block(
            index=len(self.chain) + 1,
            timestamp=time.time(),
            transactions=self.current_transactions,
            proof=proof,
            previous_hash=previous_hash or self.last_block.hash(),
            validator=self.node_address
        )
        
        self.current_transactions = []
        
        self.chain.append(block)
        return block
    
    @property
    def last_block(self) -> Block:
        return self.chain[-1]
    
    def new_transaction(self, sender: str, recipient: str, amount: float, 
                       fee: float, signature: str, contract_call: ContractCall = None) -> int:
        transaction = Transaction(
            sender=sender,
            recipient=recipient,
            amount=amount,
            fee=fee,
            signature=signature,
            contract_call=contract_call
        )
        
        if not transaction.validate():
            raise ValueError("Invalid transaction signature")
            
        self.current_transactions.append(transaction)
        return self.last_block.index + 1
    
    def proof_of_stake(self) -> str:
        total_stake = sum(self.stakes.values())
        if total_stake == 0:
            return self.node_address
            
        target = random.uniform(0, total_stake)
        current = 0
        for address, stake in self.stakes.items():
            current += stake
            if current >= target:
                return address
        return self.node_address
    
    def validate_chain(self, chain: List[Block]) -> bool:
        for i in range(1, len(chain)):
            if chain[i].previous_hash != chain[i-1].hash():
                return False
        return True
    
    def deploy_contract(self, contract_class: type, address: str, owner: str) -> str:
        contract = contract_class(address, owner)
        self.contracts[address] = contract
        return address
    
    def execute_contract(self, contract_address: str, method: str, args: List[Any], 
                         sender: str, value: float = 0) -> Any:
        if contract_address not in self.contracts:
            raise ValueError("Contract not found")
        return self.contracts[contract_address].execute(method, args, sender, value)

class P2PNetwork:
    def __init__(self, blockchain: Blockchain):
        self.blockchain = blockchain
        self.app = FastAPI()
        self.session = aiohttp.ClientSession()
        self.setup_api()
        
    def setup_api(self):
        @self.app.post("/transaction")
        async def receive_transaction(transaction: dict):
            try:
                pass
            except Exception as e:
                raise HTTPException(status_code=400, detail=str(e))
        
        @self.app.get("/chain")
        async def get_chain():
            return [asdict(block) for block in self.blockchain.chain]
        
        @self.app.post("/register_node")
        async def register_node(node_address: str):
            self.blockchain.peers.add(node_address)
            return {"status": "success"}
    
    async def broadcast_transaction(self, transaction: Transaction):
        tasks = []
        for peer in self.blockchain.peers:
            task = asyncio.create_task(
                self.session.post(
                    f"http://{peer}/transaction",
                    json=asdict(transaction)
                )
            )
            tasks.append(task)
        await asyncio.gather(*tasks, return_exceptions=True)
    
    async def sync_chain(self):
        for peer in self.blockchain.peers:
            async with self.session.get(f"http://{peer}/chain") as resp:
                if resp.status == 200:
                    peer_chain = await resp.json()
                    pass

#Запуск ноды

async def run_node(port: int):
    blockchain = Blockchain()
    network = P2PNetwork(blockchain)
    

    config = uvicorn.Config(
        network.app,
        host="0.0.0.0",
        port=port,
        log_level="info"
    )
    server = uvicorn.Server(config)
    

    async with ThreadPoolExecutor() as pool:
        await asyncio.gather(
            asyncio.to_thread(server.run),
            network.sync_chain(),
        )

if __name__ == "__main__":
    import sys
    port = int(sys.argv[1]) if len(sys.argv) > 1 else 8000
    asyncio.run(run_node(port))
