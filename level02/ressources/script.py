hex_values = [
    "756e505234376848",
    "45414a3561733951",
    "377a7143574e6758",
    "354a35686e475873",
    "48336750664b394d"
]

result = ''.join(
    bytes.fromhex(h)[::-1].decode('ascii', errors='ignore') 
    for h in hex_values
)

print(result)