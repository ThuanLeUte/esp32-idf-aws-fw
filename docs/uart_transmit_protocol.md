# UART transmit Protocol

## Wifi Information

- Json Format:
```
{
   "operation":"wifi",
   "ssid":"ssid"
   "password":"password"
}
```
## AWS Certificates

- Json Format:
```
{
   "operation":"certs",
   "certs_type":"certs_type"
   "data":"data"
}

"certs_type":
1. "certificatePem"
2. "privateKey"
3. "rootCa"
```

## AWS Endpoint
- Json Format:
```
{
   "operation":"aws_endpoint",
   "endpoint":"endpoint"
}