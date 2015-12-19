#include <Ethernet.h>
#include <ctype.h>
#include <stdint.h>

/*
 * The 'NodeAssassin' code without comments, for future reference. 
 * Origin: https://github.com/digimer/Node-Assassin/blob/master/naos.c
 */


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF };
byte ip[] = { 192, 168, 1, 66 };
byte nm[] = { 255, 255, 255, 0 };
byte dg[] = { 192, 168, 1, 1 };


#define PORT 238
Server server = Server(PORT);

#define NODECOUNT    5
#define FIRSTNODEPIN 2


void printError(const char *message);
void printMessage(const char *message);

void setup()
{

    Ethernet.begin(mac, ip, dg, nm);

    Serial.begin(9600);
    Serial.println("Node Assassin: 'Ariel' now listening for orders.");

    for (int pin = FIRSTNODEPIN; pin < (FIRSTNODEPIN+NODECOUNT); pin++)
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }

    server.begin();
}


void loop() {

    uint8_t node=0;
    uint8_t state=0;
    char nodeASCII[3];

    char command[5];
    int  index = 0;

    Client client=server.available();
    if (client) {
        while ((-1 != (command[index] = client.read()) ) && (5 > index)) {
            if( ('\n' == command[index]) || ('\r' == command[index]) ) {
                break;
            }
            index++;
        }

        if (0 == index) {
            return;
        }

        if (4 > index) {
            printMessage("Message too short. Format is 'XX:Y' where 'XX' is the zero-padded node number and Y is the state to set.\n");
            return;
        }

        if (5 == index) {
            char ch;
            printMessage("Message too long. Format is 'XX:Y' where 'XX' is the zero-padded node number and Y is the state to set.\n");
            while (-1 != (ch = client.read()) ) { 
                if ( ('\n' == ch) || ('\r' == ch) ) {
                    break;
                }
            }
            return;
        }

        command[index] = 0;

        if (':' != command[2]) { 
            printError(command);
            return;
        }

        if (!isdigit(command[0]) || !isdigit(command[1]) || !isdigit(command[3])) { 
            printError(command);
            return;
        } 

        node=command[0]-'0';
        node*=10;
        node+=command[1]-'0';

        state=command[3]-'0';

        nodeASCII[0] = command[0];
        nodeASCII[1] = command[1];
        nodeASCII[2] = 0;

        if (node > NODECOUNT) { 
            nodeASCII[0]=(NODECOUNT/10)+'0';
            nodeASCII[1]=(NODECOUNT%10)+'0';

            printMessage("This fence only supports up to "); printMessage(nodeASCII); printMessage("nodes.\n");
            return;
        }


        if (state > 1) {
            printMessage("Invalid state received. Send 'XX:0' to kill a node, XX:1 to release a node\n");
            return;
        }


        if (0 == node) {
            printMessage("Node states: \n");

            nodeASCII[0]=(NODECOUNT/10)+'0';
            nodeASCII[1]=(NODECOUNT%10)+'0';

            printMessage("- Max Node: "); printMessage(nodeASCII); printMessage("\n");

            for (int i=0; i<NODECOUNT; i++) {
                nodeASCII[0]=((i+1)/10)+'0';
                nodeASCII[1]=((i+1)%10)+'0';

                state = digitalRead(i+FIRSTNODEPIN);
                printMessage("- Node "); printMessage(nodeASCII); printMessage((LOW == state) ? ": Running\n" : ": Fenced!\n");
            }

            printMessage("End Message.\n");

            return;
        }

        node--;

        digitalWrite(node+FIRSTNODEPIN, (0 == state) ? HIGH : LOW);
        printMessage("Node "); printMessage(command); printMessage((1 == state) ? ": Now running.\n" : ": Now Fenced!\n");
    }
}


void printError(const char *message) {
    printMessage("Bad command: [" ); printMessage(message); printMessage("]\n" );
}

void printMessage(const char *message) {
    Serial.print(message);
    server.write(message);
}
