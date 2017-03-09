import socket
import sys

usage = """
Send a UDP Datagram of <message> to the specified IP address on port 8888.
Usage:
  {script} ipaddress message
""".format(script=sys.argv[0])
if(len(sys.argv)< 2):
    sys.exit(usage)

UDP_IP = sys.argv[1]
UDP_PORT = 8888
MESSAGE = sys.argv[2]

print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT
print "message:", MESSAGE

sock = socket.socket(socket.AF_INET, # Internet
                 socket.SOCK_DGRAM) # UDP
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
