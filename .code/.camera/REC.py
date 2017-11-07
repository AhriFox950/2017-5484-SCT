import socket
# import SEN

accepted = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9']
DATA = {'UDP_IP' : 'localhost', 'UDP_PORT' : '5005', 'UDP2_PORT' : '5007', 'UDP_TARGET' : '10.54.84.50', 'UDP_TARGET_PORT' : '5006', 'UDP2_TARGET' : '172.22.11.2', 'UDP2_TARGET_PORT' : '5007',
        'action' : 0}
        
def BStringParse(toParse):
    toReturn = ''
    times = 0

    while (times < len(toParse)):
        if (toParse[times] in accepted):
            toReturn += toParse[times]
        times += 1

    if (toReturn != ""):
       return toReturn
    else:
        return ''

def SplitAt(_SplitAt, toSplit):
    return toSplit[_SplitAt:_SplitAt+3]

def Split4At(_SplitAt, toSplit):
    return toSplit[_SplitAt:_SplitAt+4]

def Main():
    # Set up the python SEN Data.
    # SEN.Set(UDP_IP, int(UDP_PORT), )
    
    UDP_IP = DATA['UDP_IP']
    UDP_PORT = DATA['UDP_PORT']

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, int(UDP_PORT)))

    sockA = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sockA.bind((DATA['UDP_IP'], int(DATA['UDP2_PORT'])))


    sock2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock3 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    #sock2.bind(('172.22.11.10', 5006))

    while (True):
        data, addr = sock.recvfrom(1024)
        
        toData = BStringParse(str(data))
        toData = SplitAt(10, toData)

        print (str('[ ' + str(DATA['action']) + ' ] ' ) + 'Recieved data: ' + toData + ' from: ' + str(addr))
        DATA['action'] += 1

        dataA, addr = sockA.recvfrom(1024)

        print (str(dataA))
        toDataA = str(dataA)
        #toDataA = BStringParse(toDataA)
        #toDataA = Split4At(12, toDataA)
        toDataA = toDataA[38:44]
        toDataA = BStringParse(toDataA)
        
        print (str('[ ' + str(DATA['action']) + ' ] ' ) + 'Recieved data: ' + toDataA + ' from: ' + str(addr))
        DATA['action'] += 1

        # Send the X:
        print (str('[ ' + str(DATA['action']) + ' ] ' ) + 'Sending  data: ' + toData + ' to  : ' + DATA['UDP_TARGET'] + ':' + DATA['UDP_TARGET_PORT'])
        sock2.sendto(bytes(toData, 'utf-8'), (DATA['UDP_TARGET'], int((DATA['UDP_TARGET_PORT']))))
        DATA['action'] += 1

        # Send the Y:
        #print (str('[ ' + str(DATA['action']) + ' ] ' ) + 'Sending  data: ' + toDataA + ' to  : ' + DATA['UDP2_TARGET'] + ':' + DATA['UDP2_TARGET_PORT'])
        #sock2.sendto(bytes(toDataA, 'utf-8'), (DATA['UDP2_TARGET'], int((DATA['UDP2_TARGET_PORT']))))
        #DATA['action'] += 1
        
Main()
