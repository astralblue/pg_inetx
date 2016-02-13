#include <postgres.h>
#include <fmgr.h>

#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_URL
#undef PACKAGE_VERSION

#include "config.h"

#include <sys/socket.h>

#include <utils/inet.h>

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

#define DEBUG_LOG(fmt, ...) elog(DEBUG5, "%s: " fmt, __func__, ## __VA_ARGS__)

PG_FUNCTION_INFO_V1(macaddr_to_ipv6_interface_id);

Datum
macaddr_to_ipv6_interface_id(PG_FUNCTION_ARGS)
{
	macaddr const *mac = PG_GETARG_MACADDR_P(0);
	inet *iid = palloc0(sizeof(*iid));
	ip_family(iid) = PGSQL_AF_INET6;
	ip_bits(iid) = 128;
	ip_addr(iid)[8] = mac->a ^ 0x02;
	ip_addr(iid)[9] = mac->b;
	ip_addr(iid)[10] = mac->c;
	ip_addr(iid)[11] = 0xff;
	ip_addr(iid)[12] = 0xfe;
	ip_addr(iid)[13] = mac->d;
	ip_addr(iid)[14] = mac->e;
	ip_addr(iid)[15] = mac->f;
	SET_INET_VARSIZE(iid);
	PG_RETURN_INET_P(iid);
}

PG_FUNCTION_INFO_V1(ipv4_to_ipv6to4);

Datum
ipv4_to_ipv6to4(PG_FUNCTION_ARGS)
{
	inet *ipv4 = PG_GETARG_INET_P(0);
	if (ip_family(ipv4) != PGSQL_AF_INET)
		PG_RETURN_NULL();
	inet *ipv6to4 = palloc0(sizeof(*ipv6to4));
	ip_family(ipv6to4) = PGSQL_AF_INET6;
	ip_bits(ipv6to4) = 48;
	ip_addr(ipv6to4)[0] = 0x20;
	ip_addr(ipv6to4)[1] = 0x02;
	ip_addr(ipv6to4)[2] = ip_addr(ipv4)[0];
	ip_addr(ipv6to4)[3] = ip_addr(ipv4)[1];
	ip_addr(ipv6to4)[4] = ip_addr(ipv4)[2];
	ip_addr(ipv6to4)[5] = ip_addr(ipv4)[3];
	SET_INET_VARSIZE(ipv6to4);
	PG_RETURN_INET_P(ipv6to4);
}

PG_FUNCTION_INFO_V1(ipv6to4_to_ipv4);

Datum
ipv6to4_to_ipv4(PG_FUNCTION_ARGS)
{
	inet *ipv6to4 = PG_GETARG_INET_P(0);
	if (ip_family(ipv6to4) != PGSQL_AF_INET6)
		PG_RETURN_NULL();
	if (ip_addr(ipv6to4)[0] != 0x20 ||
	    ip_addr(ipv6to4)[1] != 0x02 ||
	    ip_bits(ipv6to4) < 48)
		PG_RETURN_NULL();
	inet *ipv4 = palloc0(sizeof(*ipv4));
	ip_family(ipv4) = PGSQL_AF_INET;
	ip_bits(ipv4) = 32;
	ip_addr(ipv4)[0] = ip_addr(ipv6to4)[2];
	ip_addr(ipv4)[1] = ip_addr(ipv6to4)[3];
	ip_addr(ipv4)[2] = ip_addr(ipv6to4)[4];
	ip_addr(ipv4)[3] = ip_addr(ipv6to4)[5];
	SET_INET_VARSIZE(ipv4);
	PG_RETURN_INET_P(ipv4);
}

static void
copy_bits(unsigned char *dst, unsigned char const *src,
	  unsigned char start, unsigned char end)
{
	DEBUG_LOG("start=%hhu end=%hhu", start, end);
	for (unsigned char bit = start; bit < end; bit = ((bit >> 3) + 1) << 3)
	{
		unsigned char index = bit >> 3;
		unsigned char mask = ((0xff >> ((bit == start) * (start % 8))) &
				      (0xff << ((bit + 8 > end) * (8 - end % 8))));
		DEBUG_LOG("bit=%hhu index=%hhu mask=%hhu", bit, index, mask);
		dst[index] = (dst[index] & ~mask) | (src[index] & mask);
	}
}

PG_FUNCTION_INFO_V1(a6_compose);

Datum
a6_compose(PG_FUNCTION_ARGS)
{
	inet *prefix = PG_GETARG_INET_P(0);
	inet *suffix = PG_GETARG_INET_P(1);
	if (ip_family(prefix) != PGSQL_AF_INET6 ||
	    ip_family(suffix) != PGSQL_AF_INET6 ||
	    ip_bits(prefix) > ip_maxbits(prefix) ||
	    ip_bits(suffix) > ip_maxbits(suffix) ||
	    ip_bits(prefix) > ip_bits(suffix))
		PG_RETURN_NULL();
	inet *composed = palloc0(sizeof(*composed));
	ip_family(composed) = PGSQL_AF_INET6;
	ip_bits(composed) = ip_bits(prefix);
	copy_bits(ip_addr(composed), ip_addr(prefix), 0, ip_bits(suffix));
	copy_bits(ip_addr(composed), ip_addr(suffix), ip_bits(suffix), ip_maxbits(composed));
	SET_INET_VARSIZE(composed);
	PG_RETURN_INET_P(composed);
}
