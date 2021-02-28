using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ElmoMotionControlComponents.Drive.EASComponents;
using System.Net;

namespace ElmoCommunicationCSharpWrapper
{
    public class ElmoCommCSharpWrapper
    {
        ~ElmoCommCSharpWrapper()
        {
            disconnect();
        }

        public unsafe bool connect(string ip, string hostIp, ushort port, byte* errMsg)
        {
            if (isConnected)
            {
                return true;
            }
            try
            {
                commInfo = DriveCommunicationFactory.CreateUDPCommunicationInfo(IPAddress.Parse(ip), IPAddress.Parse(hostIp), port);
                communication = DriveCommunicationFactory.CreateCommunication(commInfo);

                IDriveErrorObject err;
                if (!communication.Connect(out err))
                {
                    copyBytes(Encoding.ASCII.GetBytes(err.ToString()), errMsg);
                    return false;
                }
                isConnected = true;
                return true;
            }
            catch (Exception e)
            {
                copyBytes(Encoding.ASCII.GetBytes(e.Message), errMsg);
                return false;
            }
        }

        public void disconnect()
        {
            if (communication != null && communication.IsConnected)
            {
                communication.Disconnect();
            }
            isConnected = false;
        }

        public unsafe bool sendCommand(string cmd, byte* rsp, byte* errMsg)
        {
            if (!isConnected)
            {
                copyBytes(Encoding.ASCII.GetBytes("Unconnected"), errMsg);
                return false;
            }
            try
            {
                string response;
                communication.SendCommand(cmd, out response, 100);
                copyBytes(Encoding.ASCII.GetBytes(response), rsp);
                return true;
            }
            catch (Exception e)
            {
                copyBytes(Encoding.ASCII.GetBytes(e.Message), errMsg);
                return false;
            }
        }

        private unsafe void copyBytes(byte[] source, byte* dest)
        {
            for (int i = 0; i < source.Length; i++)
            {
                dest[i] = source[i];
            }
        }

        private IDriveCommunication communication = null;
        private IDriveCommunicationInfo commInfo = null;
        private bool isConnected = false;
    }
}
