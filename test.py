import socket, sys, pprint, os, time

NORMAL="\033[0m"
BLINK="\033[5m"
BLUE="\033[34m"
MAGENTA="\033[35m"
CYAN="\033[36m"
WHITE="\033[37m"
RED="\033[31m"
GREEN="\033[32m"
PINK="\033[35m\033[1m"
BROWN="\033[33m"
YELLOW="\033[33m\033[1m"
BOLDBLACK="\033[30m\033[1m"
BOLDGREEN="\033[32m\033[1m"
BOLDBLUE="\033[34m\033[1m"
BOLDMAGENTA="\033[35m\033[1m"
BOLDCYAN="\033[36m\033[1m"
BOLDWHITE="\033[37m\033[1m"
BOLDRED="\033[31m\033[1m"
BOLDPINK="\033[35m\033[1m"
BLINKBLUE="\033[34m\033[5m"
BLINKMAGENTA="\033[35m\033[5m"
BLINKCYAN="\033[36m\033[5m"
BLINKWHITE="\033[37m\033[5m"
BLINKRED="\033[31m\033[5m"
BLINKGREEN="\033[32m\033[5m"
BLINKBROWN="\033[33m\033[5m"
BLINKYELLOW="\033[33m\033[1m\033[5m"
BLINKBBLACK="\033[30m\033[1m\033[5m"
BLINKBGREEN="\033[32m\033[1m\033[5m"
BLINKBBLUE="\033[34m\033[1m\033[5m"
BLINKBMAGENTA="\033[35m\033[1m\033[5m"
BLINKBCYAN="\033[36m\033[1m\033[5m"
BLINKBWHITE="\033[37m\033[1m\033[5m"
BLINKBRED="\033[31m\033[1m\033[5m"
BGBLUE="\033[44m"
BGBLACK="\033[40m"
BGRED="\033[41m"
BGGREEN="\033[42m"
BGYELLOW="\033[43m"
BGMAGENTA="\033[45m"
BGCYAN="\033[46m"
BGWHITE="\033[47m"

def test(test_name, text_to_send, code):
    s = socket.create_connection((sys.argv[1], sys.argv[2]))
    s.send(text_to_send)
    response = s.recv(2028)
    f_line = response.split("\r\n")[0]
    try:
        if f_line.split(" ")[1] == code:
            print test_name + ": " + GREEN + "OK" + NORMAL
        else:
            print "\n" + PINK + "##################" + NORMAL
            print test_name + ": " + BGRED + "Error BYE" + NORMAL
            time.sleep(1)
            print BGBLUE + "Send: " + NORMAL + text_to_send
            print BGYELLOW + "Recv: " + NORMAL + response + PINK + "\n#####################" + NORMAL + "\n\n\n" 
            time.sleep(1)
    except:
        print "Error Fatal ...\n" + response + "\n\n"


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print 'Usage: python test.py IP PORT'
        sys.exit(-1)
    test("test 'no error'", "GET /index.php HTTP/1.1\r\nhost: toto\r\n\r\n", "200")
    test("test '505' (HTTP/1.fion)", "GET / HTTP/1.fion\r\nhost: toto\r\n\r\n", "505")
    test("test '404' (GET /l33t/pr0n.avi)", "GET /l33t/pr0n.avi HTTP/1.1\r\nhost: toto\r\n\r\n", "404")
    test("test '301 Moved Permanently' (/ -> /index.html)", "GET / HTTP/1.1\r\nhost: toto\r\n\r\n", "301")
    test("test '405 Method Not Allowed' (GETTER /index.php HTTP/1.1)", "GETTER /index.php HTTP/1.1\r\nhost: toto\r\n\r\n", "405")
    test("test method OPTIONS 'OPTIONS * HTTP/1.1'", "OPTIONS * HTTP/1.1\r\nhost: toto\r\n\r\n", "200")
    test("test method HEAD 'HEAD / HTTP/1.1'", "HEAD / HTTP/1.1\r\nhost: toto\r\n\r\n", "200")
    test("test method PUT 'PUT / HTTP/1.1'", "PUT / HTTP/1.1\r\nhost: toto\r\n\r\n", "200")
    test("test method POST 'POST / HTTP/1.1'", "POST / HTTP/1.1\r\nhost: toto\r\n\r\n", "200")
    test("test method TRACE 'TRACE / HTTP/1.1'", "TRACE / HTTP/1.1\r\nhost: toto\r\n\r\n", "200")
    test("test method CONNECT 'CONNECT / HTTP/1.1'", "CONNECT / HTTP/1.1\r\nhost: toto\r\n\r\n", "200")
    test("test method DELETE 'DELETE / HTTP/1.1'", "DELETE / HTTP/1.1\r\nhost: toto\r\n\r\n", "200")

#     test("test simple 'no error'", "GET /index.php HTTP/1.1\r\nhost: toto\r\n\r\n", "200")

    
    
