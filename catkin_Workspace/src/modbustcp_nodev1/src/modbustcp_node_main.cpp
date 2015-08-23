#include <iostream>
#include <stdlib.h>
#include <modbus/modbus.h>
#include <unistd.h>
#include <string.h>
#include "ros/ros.h"
#include "std_msgs/UInt16MultiArray.h"
#include "std_msgs/ByteMultiArray.h"
using namespace std;

#define BITS_NUM 500  //<2000
#define REGISTERS_NUM  125 //<125

int main(int argc, char **argv)
{
    string NAME_param("PLC1_Connection");
    string IP_param("192.168.1.102");
    int PORT_param=1502;
    int ADRR_BITS_param=100;
    int ADRR_REGISTERS_param=300;

    modbus_t *ctx;
    unsigned char bits_buf[BITS_NUM];
    short unsigned int registers_buf[REGISTERS_NUM];
    int error_bits_cnt=0;
    int error_registers_cnt=0;

    vector<signed char> bits_vector(BITS_NUM);
    vector<short unsigned int> registers_vector(REGISTERS_NUM);

    std_msgs::ByteMultiArray bits_msg;
    std_msgs::UInt16MultiArray registers_msg;

    ros::init(argc,argv,"ABCD");
    ros::NodeHandle n;
    ros::Publisher bits_pub = n.advertise<std_msgs::ByteMultiArray>("PLC1bits",10);
    ros::Publisher registers_pub = n.advertise<std_msgs::UInt16MultiArray>("PLC1registers",10);
    ros::Rate loop_rate(20);


    ctx=modbus_new_tcp((char*)(IP_param.data()),PORT_param);

    if(ctx==NULL)
    {
        cout << "Unable to creat modbus handle!" <<"--"<<NAME_param<< endl;
        exit(1);
    }
    else
       cout << "Creat modbus handle" <<"--"<<NAME_param<< endl;


    while(ros::ok())
    {
        cout << "Modbus Connecting..." <<"--"<<IP_param<<"--"<<PORT_param<<"--"<<NAME_param<<endl;
        while(modbus_connect(ctx)==-1)
        {
           cout << "Connection Failed!!!" <<"--"<<IP_param<<"--"<<PORT_param<<"--"<<NAME_param<<endl;
           sleep(1);
        }

        cout << "Connection Succeed" <<"--"<<IP_param<<"--"<<PORT_param<<"--"<<NAME_param<<endl;

        while(ros::ok())
        {

            if(modbus_read_bits(ctx,ADRR_BITS_param,BITS_NUM,bits_buf)==-1)
            {
                error_bits_cnt++;
                if(error_bits_cnt>5)
                {
                    cout << "Modbus Bits Read Error!" << "--"<<NAME_param<<endl;
                    error_bits_cnt=0;
                    break;
                }
            }
            else
            {
                error_bits_cnt=0;
                for(int i=0;i<BITS_NUM;i++)
                    bits_vector[i]=(signed char)bits_buf[i];
                bits_msg.data=bits_vector;
                bits_pub.publish(bits_msg);
            }

            if(modbus_read_registers(ctx,ADRR_REGISTERS_param,REGISTERS_NUM,registers_buf)==-1)
            {
                error_registers_cnt++;
                if(error_registers_cnt>5)
                {
                    cout << "Modbus Registers Read Error!" << "--"<<NAME_param<<endl;
                    error_registers_cnt=0;
                    break;
                }
            }
            else
            {
                error_registers_cnt=0;

                for(int i=0;i<REGISTERS_NUM;i++)
                    registers_vector[i]=(int)registers_buf[i];
                registers_msg.data=registers_vector;
                registers_pub.publish(registers_msg);

            }

            loop_rate.sleep();
        }
    }


    return 0;
}

