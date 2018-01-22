/* lzo1b_cr.ch -- implementation of the LZO1B compression algorithm

   This file is part of the LZO-Professional data compression library.

   Copyright (C) 2006 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2005 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2004 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2003 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2002 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2001 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2000 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1999 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1998 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1997 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1996 Markus Franz Xaver Johannes Oberhumer
   All Rights Reserved.

   CONFIDENTIAL & PROPRIETARY SOURCE CODE.

   ANY USAGE OF THIS FILE IS SUBJECT TO YOUR LICENSE AGREEMENT.

   Markus F.X.J. Oberhumer
   <markus@oberhumer.com>
   http://www.oberhumer.com/products/lzo-professional/
 */


/* WARNING: this file should *not* be used by applications. It is
   part of the implementation of the library and is subject
   to change.
 */



/***********************************************************************
// store the current literal run
************************************************************************/

        assert(ip < ip_end);
        if (pd(ip,ii) > 0)
        {
            lzo_uint t = pd(ip,ii);

#if defined(LZO_HAVE_R1)
            if (ip == r1)
            {
                /* Code a context sensitive R1 match. */
                LZO_STATS(lzo_stats->literals += t);
                LZO_STATS(lzo_stats->r1_matches++);
                assert(t == 1);
                /* modify marker byte */
                assert((op[-2] >> M2O_BITS) == (M2_MARKER >> M2O_BITS));
                op[-2] &= M2O_MASK;
                assert((op[-2] >> M2O_BITS) == 0);
                /* copy 1 literal */
                *op++ = *ii++;
                r1 = ip + (M2_MIN_LEN + 1);     /* set new R1 pointer */
            }
            else
#endif
            if (t < R0MIN)
            {
                /* inline the copying of a short run */
                LZO_STATS(lzo_stats->literals += t);
                LZO_STATS(lzo_stats->lit_runs++);
                LZO_STATS(lzo_stats->lit_run[t]++);
#if defined(LZO_HAVE_M3)
                if (t < LZO_SIZE(8-M3O_BITS) && op == m3)
                {
                /* Code a very short literal run into the low offset bits
                 * of the previous M3/M4 match.
                 */
                    LZO_STATS(lzo_stats->lit_runs_after_m3_match++);
                    LZO_STATS(lzo_stats->lit_run_after_m3_match[t]++);
                    assert((m3[-2] >> M3O_BITS) == 0);
                    m3[-2] |= LZO_BYTE(t << M3O_BITS);
                }
                else
#endif
                {
                    *op++ = LZO_BYTE(t);
                }
                MEMCPY_DS(op, ii, t);
#if defined(LZO_HAVE_R1)
                r1 = ip + (M2_MIN_LEN + 1);     /* set new R1 pointer */
#endif
            }
            else if (t < R0FAST)
            {
                /* inline the copying of a short R0 run */
                LZO_STATS(lzo_stats->literals += t);
                LZO_STATS(lzo_stats->r0short_runs++);
                *op++ = 0; *op++ = LZO_BYTE(t - R0MIN);
                MEMCPY_DS(op, ii, t);
#if defined(LZO_HAVE_R1)
                r1 = ip + (M2_MIN_LEN + 1);     /* set new R1 pointer */
#endif
            }
            else
            {
                op = STORE_RUN(op,ii,t);
                ii = ip;
            }
        }


        /* ii now points to the start of the current match */
        assert(ii == ip);


/*
vi:ts=4:et
*/
