#include "mbed.h"
#include "mbed_rpc.h"

DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
BufferedSerial pc(USBTX, USBRX);
void LEDControl(Arguments *in, Reply *out);
RPCFunction rpcLED(&LEDControl, "LEDControl");
double x, y;
Thread t1, t2;


void Blink1(void) {
    while(1) {
        if (x) {
            myled1 = !myled1;
        } else {
            myled1 = 0;
        }
        ThisThread::sleep_for(300ms);
    }
}
void Blink2(void) {
    while(1) {
        if (y) {
            myled2 = !myled2;
        } else {
            myled2 = 0;
        }
        ThisThread::sleep_for(300ms);
    }
}

int main() {
    //The mbed RPC classes are now wrapped to create an RPC enabled version - see RpcClasses.h so don't add to base class
    // receive commands, and send back the responses
    char buf[256], outbuf[256];
    FILE *devin = fdopen(&pc, "r");
    FILE *devout = fdopen(&pc, "w");

    while(1) {
        t1.start(&Blink1);
        t2.start(&Blink2);

        memset(buf, 0, 256);
        for (int i = 0; ; i++) {
            char recv = fgetc(devin);
            if (recv == '\n') {
                printf("\r\n");
                break;
            }
            buf[i] = fputc(recv, devout);
        }

        //Call the static call method on the RPC class
        RPC::call(buf, outbuf);
        printf("%s\r\n", outbuf);

    }

}
// Make sure the method takes in Arguments and Reply objects.

void LEDControl (Arguments *in, Reply *out)   {
    // In this scenario, when using RPC delimit the two arguments with a space.
    x = in->getArg<double>();
    y = in->getArg<double>();
}

