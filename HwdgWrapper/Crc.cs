using System;
using System.Linq;

namespace HwdgWrapper
{
    public static class Crc
    {
        /// <summary>
        /// Precalculated values table.
        /// </summary>
        private static readonly Byte[] Table = new Byte[256];

        /// <summary>
        /// x7 + x3 + x0
        /// </summary>
        private const Byte PolyCrc7 = 0x89;

        /// <summary>
        /// ctor
        /// </summary>
        static Crc()
        {
            // Fill in CRC table.
            for (var i = 0; i < 256; i++)
            {
                var x = i;
                for (var j = 0; j < 8; j++)
                    x = (x & 0x80) != 0 ? (x << 1) ^ PolyCrc7 : x << 1;
                Table[i] = (Byte) x;
            }
        }

        /// <summary>
        /// Calculate checksum with x7 + x3 + x0 polynomial.
        /// </summary>
        /// <param name="data">Byte array we wanna calculate checksum.</param>
        /// <returns>Returns checksum calculated with x7 + x3 + x0 polynomial.</returns>
        public static Byte CalcCrc7(this Byte[] data)
        {
            if (data == null) throw new ArgumentNullException(nameof(data));
            if (data.Length == 0) throw new ArgumentException("Value cannot be an empty collection.", nameof(data));
            const Byte crc = 0;
            return data.Aggregate(crc, (x, b) => Table[x ^ b]);
        }


        /// <summary>
        /// Calculate checksum with x7 + x3 + x0 polynomial.
        /// </summary>
        /// <param name="data">Byte array we wanna calculate checksum.</param>
        /// <param name="count">Number of bytes in array involved in checksum calculation.</param>
        /// <returns>Returns checksum calculated with x7 + x3 + x0 polynomial.</returns>
        public static Byte CalcCrc7(this Byte[] data, Byte count)
        {
            if (data == null) throw new ArgumentNullException(nameof(data));
            if (data.Length == 0) throw new ArgumentException("Value cannot be an empty collection.", nameof(data));
            if (count > data.Length) throw new ArgumentOutOfRangeException(nameof(count));

            Byte crc = 0;
            for (var i = 0; i < count; i++)
                crc = data[crc ^ data[i]];
            return crc;
        }
    }
}