#include <r_util.h>

R_API RRangeTiny *r_tinyrange_new() {
	return R_NEW0 (RRangeTiny);
}

R_API void r_tinyrange_init(RRangeTiny *bbr) {
	bbr->pairs = 0;
	bbr->ranges = NULL;
}

R_API void r_tinyrange_fini(RRangeTiny *bbr) {
	bbr->pairs = 0;
	R_FREE (bbr->ranges);
}

R_API bool r_tinyrange_in(RRangeTiny *bbr, ut64 at) {
	int i;
	if (bbr->pairs > 0) {
		int lastIndex = (bbr->pairs * 2);
		if (at < bbr->ranges[0]) {
			return false;
		}
		if (at > bbr->ranges[lastIndex + 1]) {
			return false;
		}
	}
	for (i = 0; i < bbr->pairs; i++) {
		int idx = i * 2;
		if (at >= bbr->ranges[idx] && at < bbr->ranges[idx + 1]) {
			return true;
		}
	}
	return false;
}

/* must be always called in a sorted way */
R_API bool r_tinyrange_add(RRangeTiny *bbr, ut64 from, ut64 to) {
	if (from >= to) {
		return false;
	}
	if (bbr->pairs > 0) {
		int idx = (bbr->pairs - 1) * 2;
		if (from == bbr->ranges[idx + 1]) {
			bbr->ranges[idx + 1] = to;
		} else {
			bbr->pairs++;
			idx += 2;
			void *ranges = realloc (bbr->ranges, sizeof (ut64) * bbr->pairs * 2);
			if (!ranges) {
				return false;
			}
			bbr->ranges = ranges;
			bbr->ranges[idx] = from;
			bbr->ranges[idx + 1] = to;
		}
	} else {
		bbr->pairs = 1;
		bbr->ranges = calloc (sizeof (ut64), 2);
		bbr->ranges[0] = from;
		bbr->ranges[1] = to;
	}
	return true;
}

#if 0
main() {
	RRangeTiny *bbr = r_tinyrange_new ();
	r_tinyrange_add (bbr, 100, 200);
	r_tinyrange_add (bbr, 300, 400);
	r_tinyrange_add (bbr, 400, 500);
	eprintf ("%d\n", r_tinyrange_in (bbr, 100));
	eprintf ("%d\n", r_tinyrange_in (bbr, 250));
	eprintf ("%d\n", r_tinyrange_in (bbr, 450));
//	r_tinyrange_free (bbr);
}
#endif
