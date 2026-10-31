#include "raf_cascade_v1.h"

/* Hosted test harness only: main/exit belongs to platform CRT, not core.
 * Return index on failure. No printf, heap, I/O, or test dependency. */
static rfc_graph base(void) {
    rfc_graph g={0};
    g.node_count=4u; g.seeds=1u; g.known_nodes=15u;
    g.max_reach=4u; g.max_depth=4u; g.max_frontier=4u;
    g.participant_count=3u; g.dimension_label=9u;
    return g;
}
static void edge(rfc_graph *g, unsigned n, unsigned from, unsigned to, unsigned relation, unsigned owner) {
    g->edges[n].from=(uint8_t)from; g->edges[n].to=(uint8_t)to;
    g->edges[n].relation=(uint8_t)relation; g->edges[n].owner=(uint8_t)owner;
}
#define REQUIRE(expr) do { if (!(expr)) return __LINE__; } while(0)
int main(void) {
    rfc_graph g=base(); rfc_report r, s;
    REQUIRE(rfc_assess(&g,&r)==RFC_ALLOW_SCOPED);                         /* 1 */
    REQUIRE(r.reached_count==1u && r.claim_allowed==0u && r.dimension_label==9u); /* 2 */
    g.edge_count=3u; edge(&g,0,0,1,0,1); edge(&g,1,1,2,0,2); edge(&g,2,2,3,0,3);
    REQUIRE(rfc_assess(&g,&r)==RFC_ALLOW_SCOPED && r.reached_count==4u);  /* 3 */
    REQUIRE(r.depth==3u && r.peak_frontier==1u);                         /* 4 */
    g.critical_nodes=8u;
    REQUIRE(rfc_assess(&g,&r)==RFC_BLOCK_CRITICAL && r.critical_mask==8u); /* 5 */
    g.critical_nodes=0u;g.known_nodes=3u;
    REQUIRE(rfc_assess(&g,&r)==RFC_BLOCK_UNKNOWN && r.unknown_mask==12u); /* 6 */
    g.known_nodes=15u;g.edges[0].relation=RFC_EDGE_UNKNOWN;
    REQUIRE(rfc_assess(&g,&r)==RFC_BLOCK_UNKNOWN && r.uncertain_edges==1u); /* 7 */
    g.edges[0].relation=RFC_EDGE_CUT;
    REQUIRE(rfc_assess(&g,&r)==RFC_ALLOW_SCOPED && r.reached_count==1u && r.cut_edges==1u); /* 8 */
    g.edges[0].relation=RFC_EDGE_CONFIRMED;
    g.max_reach=2u;
    REQUIRE(rfc_assess(&g,&r)==RFC_BLOCK_REACH && r.rollback_required==1u); /* 9 */
    g.max_reach=4u;g.max_depth=2u;
    REQUIRE(rfc_assess(&g,&r)==RFC_BLOCK_DEPTH);                          /* 10 */
    g.max_depth=4u;g.max_frontier=1u;
    edge(&g,0,0,1,0,1);edge(&g,1,0,2,0,2);edge(&g,2,0,3,0,3);
    REQUIRE(rfc_assess(&g,&r)==RFC_BLOCK_FRONTIER && r.peak_frontier==3u); /* 11 */
    g=base();g.edge_count=2u; edge(&g,0,0,1,0,1);edge(&g,1,1,0,0,2);
    REQUIRE(rfc_assess(&g,&r)==RFC_ALLOW_SCOPED && r.reached_count==2u); /* 12 cycle */
    REQUIRE(rfc_assess(&g,&s)==RFC_ALLOW_SCOPED && s.reached_mask==r.reached_mask); /* 13 */
    g.edges[1].to=4u;
    REQUIRE(rfc_assess(&g,&r)==RFC_BLOCK_INVALID);                       /* 14 */
    g=base();g.seeds=0u;
    REQUIRE(rfc_assess(&g,&r)==RFC_BLOCK_INVALID);                       /* 15 */
    g=base();g.edge_count=1u;edge(&g,0,0,1,0,0);g.require_edge_owner=1u;
    REQUIRE(rfc_assess(&g,&r)==RFC_BLOCK_UNKNOWN);                       /* 16 */
    g.require_edge_owner=0u;
    REQUIRE(rfc_assess(&g,&r)==RFC_ALLOW_SCOPED);                        /* 17 */
    REQUIRE(rfc_assess((const rfc_graph *)0,&r)==RFC_BLOCK_INVALID);     /* 18 */
    REQUIRE(rfc_assess(&g,(rfc_report *)0)==RFC_BLOCK_INVALID);         /* 19 */
    g=base();g.node_count=32u;g.known_nodes=UINT32_MAX;g.seeds=0x80000000u;
    REQUIRE(rfc_assess(&g,&r)==RFC_ALLOW_SCOPED && r.reached_mask==0x80000000u); /* 20 */
    g=base();g.participant_count=4u;
    REQUIRE(rfc_assess(&g,&r)==RFC_BLOCK_INVALID);                       /* 21 */
    g=base();g.dimension_label=18u;
    REQUIRE(rfc_assess(&g,&r)==RFC_ALLOW_SCOPED && r.dimension_label==18u); /* 22 */
    return 0;
}
