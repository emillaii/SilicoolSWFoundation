using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ElmoCommunicationCSharpWrapper
{
    public class ElmoWrapperManager
    {
        public static void createInstance(string name)
        {
            if (!wrappers.ContainsKey(name))
            {
                wrappers[name] = new ElmoCommCSharpWrapper();
            }
        }

        public static void deleteInstance(string name)
        {
            if (wrappers.ContainsKey(name))
            {
                wrappers[name].disconnect();
                wrappers.Remove(name);
            }
        }

        public static unsafe bool connect(string name, string ip, string hostIp, ushort port, byte* errMsg)
        {
            return wrappers[name].connect(ip, hostIp, port, errMsg);

        }

        public static unsafe void disconnect(string name)
        {
            wrappers[name].disconnect();
        }

        public static unsafe bool sendCommand(string name, string cmd, byte* rsp, byte* errMsg)
        {
            return wrappers[name].sendCommand(cmd, rsp, errMsg);
        }

        private static Dictionary<string, ElmoCommCSharpWrapper> wrappers = new Dictionary<string, ElmoCommCSharpWrapper>();
    }
}
