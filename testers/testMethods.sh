#!/bin/bash

SERVER_URL="http://127.0.0.1:8080/"

test_request() {
    local METHOD="$1"
    local EXPECTED_STATUS="$2"
    local RESPONSE_CONTENT="$3"

    echo "Testing $METHOD request..."

    RESPONSE=$(curl -s -X "$METHOD" -w "%{http_code}" -d "$RESPONSE_CONTENT" "$SERVER_URL")
    HTTP_STATUS="${RESPONSE:(-3)}"

    if [ "$HTTP_STATUS" -eq "$EXPECTED_STATUS" ]; then
        echo "$METHOD request successful - Status code: $HTTP_STATUS"
    else
        echo "$METHOD request failed - Status code: $HTTP_STATUS"
    fi
}

# GET
test_request "GET" 200 ""

# POST
test_request "POST" 200 '{"key": "value"}'

# Unknown
test_request "UNKNOWN_METHOD" 405 ""


SERVER_SECOND_URL="http://127.0.0.1:8080"

# DELETE
echo "Testing DELETE request"
FILE_TO_DELETE="/toDelete/delet.html"
RESPONSE=$(curl -s -w "%{http_code}" -X DELETE "$SERVER_URL/$FILE_TO_DELETE")
HTTP_STATUS="${RESPONSE:(-3)}"

if [ "$HTTP_STATUS" -eq 200 ]; then
    echo "DELETE request successful - Status code: $HTTP_STATUS"
else
    echo "DELETE request failed - Status code: $HTTP_STATUS"
fi