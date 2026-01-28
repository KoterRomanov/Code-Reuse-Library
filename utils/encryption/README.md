# Encryption and Decryption

This directory contains encryption, decryption, and cryptographic utilities.

## Contents

- **Symmetric Encryption**: AES, DES, 3DES encryption
- **Asymmetric Encryption**: RSA, ECC encryption
- **Hashing**: MD5, SHA-1, SHA-256, SHA-512, bcrypt
- **Password Hashing**: Secure password storage (bcrypt, argon2)
- **Digital Signatures**: Sign and verify data
- **Key Generation**: Generate encryption keys
- **Key Exchange**: Diffie-Hellman key exchange
- **Salt Generation**: Generate cryptographic salts
- **Random Number Generation**: Cryptographically secure random
- **Base64 Encoding**: Encode/decode Base64
- **JWT Handling**: Create and verify JSON Web Tokens
- **Certificate Management**: Handle X.509 certificates
- **HMAC**: Hash-based message authentication

## Features

- Industry-standard algorithms
- Secure defaults
- Key management utilities
- Padding schemes
- Multiple cipher modes

## Usage Examples

```python
# Symmetric encryption
encrypted = aes_encrypt(data, key)
decrypted = aes_decrypt(encrypted, key)

# Password hashing
hashed = hash_password('mypassword')
is_valid = verify_password('mypassword', hashed)

# Digital signature
signature = sign_data(data, private_key)
is_valid = verify_signature(data, signature, public_key)

# Generate keys
public_key, private_key = generate_rsa_keypair(2048)
```
