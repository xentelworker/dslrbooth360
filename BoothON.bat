:: Wait for received event and params for countdown start.
IF "%~1"=="countdown_start" ( 

 :: Send serial command to Arudino for 360 operation.
  "%~dp0serialsend.exe" /baudrate 9600 /devnum 4 "BOOTH_ON"

)