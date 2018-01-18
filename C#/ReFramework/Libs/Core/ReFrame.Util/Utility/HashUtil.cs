using System.Text;

namespace ReFrame.Util.Utility
{
    public static class HashUtil
    {
        public static uint GetHash(StringBuilder str)
        {
            if (str == null) return 0;

            uint hash = 0;
            for (int i = 0; i < str.Length; i++)
            {
                hash = (hash << 5) + hash + str[i];
            }

            return hash;
        }

        /// <summary>
        /// times33算法（快速哈希算法）
        /// <![CDATA[
        /// 不断的乘33 nHash = nHash*33 + *key++
        /// seed2 + (seed2 << 5) 相当于乘以33
        /// ]]>
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        public static uint GetHash(string str)
        {
            if (str == null) return 0;

            uint hash = 0;
            for (int i = 0; i < str.Length; i++)
            {
                hash = (hash << 5) + hash + str[i];
            }

            return hash;
        }
    }
}