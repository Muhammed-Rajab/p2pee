#!/bin/sh

# check if argument is missing
if [ -z "$1" ]; then
  echo "Usage: $0 <key-name>"
  exit 1
fi

openssl genpkey -algorithm RSA -out "$1.pem" -pkeyopt rsa_keygen_bits:2048
openssl rsa -pubout -in "$1.pem" -out "$1-public.pem"
