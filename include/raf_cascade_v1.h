#ifndef RAFCODE_CASCADE_V1_H
#define RAFCODE_CASCADE_V1_H
#include <stdint.h>

/* Source of contract: termux-packagesMy@c56d34d330ff8ca621c93bbe8c36d6ab52f3844e.
 * Standalone authorial implementation; no vendored Termux or platform code.
 * Fixed-capacity conservative directed dependency graph, no allocator / libc / I/O.
 */
#define RFC_NODE_MAX 32u
#define RFC_EDGE_MAX 96u

#define RFC_EDGE_CONFIRMED 0u
#define RFC_EDGE_UNKNOWN   1u
#define RFC_EDGE_CUT       2u

typedef enum {
    RFC_ALLOW_SCOPED = 0,
    RFC_BLOCK_INVALID = 1,
    RFC_BLOCK_UNKNOWN = 2,
    RFC_BLOCK_CRITICAL = 3,
    RFC_BLOCK_REACH = 4,
    RFC_BLOCK_DEPTH = 5,
    RFC_BLOCK_FRONTIER = 6
} rfc_status;

typedef struct {
    uint8_t from, to;
    uint8_t relation; /* CONFIRMED, UNKNOWN, explicitly CUT */
    uint8_t owner;    /* 0=unknown provenance; 1..3=known participant */
} rfc_edge;

typedef struct {
    uint32_t node_count, edge_count;
    uint32_t seeds, known_nodes, critical_nodes;
    uint32_t max_reach, max_depth, max_frontier;
    uint8_t participant_count; /* 0=not provided, 1..3 otherwise */
    uint8_t dimension_label;   /* opaque D6/D9/D12/D18 label or 0=unknown */
    uint8_t require_edge_owner; /* explicit gate; do not infer from Dn */
    uint8_t reserved;
    rfc_edge edges[RFC_EDGE_MAX];
} rfc_graph;

typedef struct {
    uint32_t reached_mask, unknown_mask, critical_mask;
    uint32_t reached_count, depth, peak_frontier;
    uint32_t uncertain_edges, cut_edges;
    uint32_t rollback_required; /* advice only; never performs filesystem action */
    uint8_t dimension_label;
    uint8_t participant_count;
    uint8_t claim_allowed; /* always false: simulator is not scientific proof */
    uint8_t reserved;
    rfc_status status;
} rfc_report;

/* Total deterministic bounded operation: does not read external data or mutate graph.
 * Returns BLOCK_UNKNOWN when an active path contains unknown source/link/owner.
 * Unknown on an unreachable path is recorded only in the source, not a blocker.
 * Passing this gate does NOT authorize deployment or certify physical SOC dynamics.
 */
rfc_status rfc_assess(const rfc_graph *graph, rfc_report *report);
#endif
