import commands
import time

#At the beginning we suppose that the lamp is off
allume = False
ipv6ServerAdd = "aaaa::xxx:xxxx:xxxx:xxxx"

#First: we discover the addresses
add = commands.getstatusoutput("wget http://["+ipv6ServerAdd+"]/ -q -O - | grep -oE \"(:*([a-z]|[0-9])+)+/128\"")
if len(add) < 60:
    print("We need at least 2 addresses")
lampAdd = add[1][:25]
veilleuseAdd = add[1][len(add[1])-29:len(add[1])-4]

#Init allume var to get the real status of the lamp
getLamp = float(commands.getstatusoutput("./coap-client -m get coap://["+ lampAdd + "]:5683/pwr/w | awk 'FNR==2{print}'")[1])
if getLamp > 3:
    allume = True

#Init veilleuse
getVeilleuse = float(commands.getstatusoutput("./coap-client -m get coap://["+ veilleuseAdd + "]:5683/pwr/rel | awk 'FNR==2{print}'")[1])
if allume and getVeilleuse == 1:  
    commands.getstatusoutput("./coap-client -m post coap://["+ veilleuseAdd +"]:5683/pwr/rel")
if not allume and getVeilleuse == 0:
    commands.getstatusoutput("./coap-client -m post coap://["+ veilleuseAdd +"]:5683/pwr/rel")

while True:
    #We get the status of the lamp
    getLamp = float(commands.getstatusoutput("./coap-client -m get coap://["+ lampAdd + "]:5683/pwr/w | awk 'FNR==2{print}'")[1])
    if getLamp < 1 and allume:
        allume = False
        commands.getstatusoutput("./coap-client -m post coap://["+ veilleuseAdd +"]:5683/pwr/rel")
        print("Turn on the night lamp")
    elif getLamp > 3 and not allume:
        allume = True
        commands.getstatusoutput("./coap-client -m post coap://["+ veilleuseAdd +"]:5683/pwr/rel")
        print("Turn off the night lamp")
    time.sleep(0.2)
