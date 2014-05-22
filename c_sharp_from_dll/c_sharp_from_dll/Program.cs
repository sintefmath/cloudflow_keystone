using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System;
using System.Runtime.InteropServices;

namespace c_sharp_from_dll
{
    class Program {
        const string keystoneDll = @"keystone.dll";

        [DllImport(keystoneDll, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern int keystone_init(string url, ref int keystone_handle);

        [DllImport(keystoneDll, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern int keystone_login(int keystone_handle, string username, string password, string tenantname, ref int info_handle);

        [DllImport(keystoneDll, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern int keystone_userinfo_get_token_buffer_size(int info_handle, ref int size);

        [DllImport(keystoneDll, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern int keystone_userinfo_get_token(int info_handle, StringBuilder token, int bufferSize, ref int dataWritten);

        [DllImport(keystoneDll, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern int keystone_userinfo_get_username_buffer_size(int info_handle, ref int size);

        [DllImport(keystoneDll, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern int keystone_userinfo_get_username(int info_handle, StringBuilder username, int bufferSize, ref int dataWritten);

        [DllImport(keystoneDll, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern int keystone_userinfo_free(int info_handle);

        [DllImport(keystoneDll, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern int keystone_free(int keystone_handle);

        [DllImport(keystoneDll, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern int keystone_get_userinfo_from_token(int keystone_handle, string tenantname, string token, ref int userinfo_handle);

        [DllImport(keystoneDll, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern int keystone_userinfo_get_role_count(int info_handle, ref int count);


        [DllImport(keystoneDll, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern int keystone_userinfo_get_role_buffer_size(int info_handle, int index, ref int size);

        [DllImport(keystoneDll, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern int keystone_userinfo_get_role(int info_handle, int index, StringBuilder role, int bufferSize, ref int dataWritten);
        

        static void Main(string[] args)
        {
            Console.WriteLine("Type in URL for keystone service: ");
            string url = Console.ReadLine();

            Console.WriteLine("Type in username: ");
            string username = Console.ReadLine();

            Console.WriteLine("Type in password: ");
            string password = Console.ReadLine();


            Console.WriteLine("Type in tenant name: ");
            string tenantname = Console.ReadLine();

            // The DLL-api work with pointers, in C# we currently just handle these as int:
            int keystone_handle = 0 ;
            
            // First we must create a keystone-object:
            Console.WriteLine("init(error) = " + keystone_init(url, ref keystone_handle));

            int info_handle = 0;
            // We log in and receive a info_handle (the info_handle will give us all information we need)
            Console.WriteLine("login(error) = " + keystone_login(keystone_handle, username, password, tenantname, ref info_handle));

            int tokenSize = 0;
            Console.WriteLine("token_buffer_size(error) = " + keystone_userinfo_get_token_buffer_size(info_handle, ref tokenSize));

            Console.WriteLine("Token size is");
            int dataWritten = 0;
            StringBuilder builder = new StringBuilder(tokenSize);

            Console.WriteLine("get_token(error) = " + keystone_userinfo_get_token(info_handle, builder, tokenSize, ref dataWritten));
            string token = builder.ToString();
            Console.WriteLine("Got token: \"" +token +"\"");
            // We are now done with the first userinfo, so let's free it:
            Console.WriteLine("userinfo_free(error) = " + keystone_userinfo_free(info_handle));

            // Based on a usertoken, we can get user info as well: 
            int info_from_token_handle = 0;
            Console.WriteLine("userinfo_from_token (error)= " + keystone_get_userinfo_from_token(keystone_handle, tenantname, token, ref info_from_token_handle));

            // To get the username, we first need to know how big it is:
            int usernameSize = 0;
            Console.WriteLine("username_buffer_size(error) = " + keystone_userinfo_get_username_buffer_size(info_from_token_handle, ref usernameSize));

            Console.WriteLine("Username size = " + usernameSize);

            // Then we create a new builder with this size: 
            StringBuilder usernameBuilder = new StringBuilder(usernameSize);
            Console.WriteLine("username(error) = " + keystone_userinfo_get_username(info_from_token_handle, usernameBuilder, usernameSize, ref dataWritten));
            Console.WriteLine("Username from token = \"" + usernameBuilder.ToString() +"\"");

            int roleCount = 0;
            Console.WriteLine("role_count(error) = " + keystone_userinfo_get_role_count(info_from_token_handle, ref roleCount));
            Console.WriteLine("roleCount = " + roleCount);

            // Get each role:
            for (int i = 0; i < roleCount; i++)
            {
                int roleBufferSize = 0;
                Console.WriteLine("role_buffer_size (error) = " + keystone_userinfo_get_role_buffer_size(info_from_token_handle, i, ref roleBufferSize));

                StringBuilder roleBuilder = new StringBuilder(roleBufferSize);
                Console.WriteLine("get_role (error) = " + keystone_userinfo_get_role(info_from_token_handle, i, roleBuilder, roleBufferSize, ref dataWritten));

                Console.WriteLine("role[" + i + "]  = " + roleBuilder.ToString());
            }

            // finally free up resources
            Console.WriteLine("info_free (error) = " + keystone_userinfo_free(info_from_token_handle));

            Console.WriteLine("keystone_free (error) = " + keystone_free(keystone_handle));
        }

        
    }
}
