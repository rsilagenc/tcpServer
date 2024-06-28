import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("127.0.0.1", 8080))
data = b"012345"
s.sendall(data)
#print("---- Received ----\n", s.recv(4096))
s.close()