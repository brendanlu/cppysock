@REM Compile windows test in this directory
g++ -D_WIN32 test_client.cpp -o test_client -lws2_32