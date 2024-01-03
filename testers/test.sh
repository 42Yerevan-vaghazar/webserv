#!/bin/bash

hostnames=("example.com" "test.com" "anotherdomain.com")
for host in "${hostnames[@]}"
do
    echo "Testing $host"
    curl_output=$(curl --resolve "$host:8080:127.0.0.1" "http://$host/")
    
    if [[ "$curl_output" == *"<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link rel='shortcut icon' href='data:image/x-icon;,' type='image/x-icon'><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
    <title>Document</title><style>body{display:flex;justify-content: center;align-items: center;}</style></head><body><h1>404</h1></body></html>"* ]]; then
        echo "Success: $host is working as expected"
    else
        echo "Error: $host is not responding as expected"
    fi
done

