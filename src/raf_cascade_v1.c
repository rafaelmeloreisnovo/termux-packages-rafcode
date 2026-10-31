#include "raf_cascade_v1.h"

static uint32_t rfc_popcount(uint32_t x) {
    uint32_t n = 0u;
    while (x != 0u) { x &= (x - 1u); ++n; }
    return n;
}

static void rfc_clear_report(rfc_report *r) {
    r->reached_mask=0u; r->unknown_mask=0u; r->critical_mask=0u;
    r->reached_count=0u; r->depth=0u; r->peak_frontier=0u;
    r->uncertain_edges=0u; r->cut_edges=0u; r->rollback_required=1u;
    r->dimension_label=0u; r->participant_count=0u;
    r->claim_allowed=0u; r->reserved=0u; r->status=RFC_BLOCK_INVALID;
}

rfc_status rfc_assess(const rfc_graph *g, rfc_report *r) {
    uint32_t valid_mask, reached, frontier, depth, uncertain=0u;
    uint32_t critical, unknown, peak, cut=0u;
    uint32_t e;
    rfc_status status;
    if (r == (rfc_report *)0) return RFC_BLOCK_INVALID;
    rfc_clear_report(r);
    if (g == (const rfc_graph *)0) return r->status;
    if (g->node_count == 0u || g->node_count > RFC_NODE_MAX ||
        g->edge_count > RFC_EDGE_MAX || g->max_reach == 0u ||
        g->max_frontier == 0u || g->max_reach > RFC_NODE_MAX ||
        g->max_frontier > RFC_NODE_MAX ||
        g->participant_count > 3u || g->require_edge_owner > 1u ||
        g->max_depth > RFC_NODE_MAX) return r->status;
    valid_mask = (g->node_count == 32u) ? UINT32_MAX : ((1u << g->node_count)-1u);
    if (g->seeds == 0u || (g->seeds & ~valid_mask) != 0u ||
        (g->known_nodes & ~valid_mask) != 0u ||
        (g->critical_nodes & ~valid_mask) != 0u) return r->status;
    for (e=0u; e<g->edge_count; ++e) {
        const rfc_edge *edge=&g->edges[e];
        if (edge->from >= g->node_count || edge->to >= g->node_count ||
            edge->relation > RFC_EDGE_CUT || edge->owner > g->participant_count)
            return r->status;
    }
    r->dimension_label=g->dimension_label;
    r->participant_count=g->participant_count;
    reached=g->seeds;
    frontier=g->seeds;
    peak=rfc_popcount(frontier);
    depth=0u;
    /* All graphs are bounded (32 nodes/96 edges). Each new layer only visits new nodes. */
    while (frontier != 0u) {
        uint32_t next=0u;
        for (e=0u; e<g->edge_count; ++e) {
            const rfc_edge *edge=&g->edges[e];
            uint32_t src=1u << edge->from;
            if ((frontier & src) == 0u) continue;
            if (edge->relation == RFC_EDGE_CUT) { ++cut; continue; }
            next |= 1u << edge->to;
            if (edge->relation == RFC_EDGE_UNKNOWN ||
                (g->require_edge_owner && edge->owner == 0u)) ++uncertain;
        }
        next &= ~reached;
        if (next == 0u) break;
        reached |= next;
        frontier=next;
        ++depth;
        if (rfc_popcount(frontier) > peak) peak=rfc_popcount(frontier);
        /* Even if a guard would trip, complete bounded closure for auditable evidence. */
    }
    unknown = reached & ~g->known_nodes;
    critical = reached & g->critical_nodes;
    r->reached_mask=reached; r->unknown_mask=unknown; r->critical_mask=critical;
    r->reached_count=rfc_popcount(reached);
    r->depth=depth; r->peak_frontier=peak;
    r->uncertain_edges=uncertain; r->cut_edges=cut;
    /* Deterministic priority: invalid > unknown > critical > capacity > depth > width.
     * Bounds of budgets still block even when no node is labelled high-risk. */
    status = (unknown != 0u || uncertain != 0u) ? RFC_BLOCK_UNKNOWN :
             critical != 0u ? RFC_BLOCK_CRITICAL :
             r->reached_count > g->max_reach ? RFC_BLOCK_REACH :
             depth > g->max_depth ? RFC_BLOCK_DEPTH :
             peak > g->max_frontier ? RFC_BLOCK_FRONTIER : RFC_ALLOW_SCOPED;
    r->status=status;
    r->rollback_required=(status != RFC_ALLOW_SCOPED);
    return status;
}
