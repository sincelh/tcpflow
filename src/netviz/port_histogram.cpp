/**
 * port_histogram.cpp: 
 * Show packets received vs port
 *
 * This source file is public domain, as it is not based on the original tcpflow.
 *
 * Author: Michael Shick <mike@shick.in>
 *
 */

#include "config.h"
#include "tcpflow.h"
#include "tcpip.h"

#include "port_histogram.h"

void port_histogram::ingest_packet(const packet_info &pi)
{
    struct tcp_seg tcp;

    if(!tcpip::tcp_from_ip_bytes(pi.ip_data, pi.ip_datalen, tcp)) {
        return;
    }

    if(relationship == SENDER || relationship == SND_OR_RCV) {
        std::stringstream ss;
        ss << ntohs(tcp.header->th_sport);
        parent_count_histogram.increment(ss.str(), 1);
    }
    if(relationship == RECEIVER || relationship == SND_OR_RCV) {
        std::stringstream ss;
        ss << ntohs(tcp.header->th_dport);
        parent_count_histogram.increment(ss.str(), 1);
    }
}

void port_histogram::render(cairo_t *cr, const plot::bounds_t &bounds)
{
#ifdef CAIRO_PDF_AVAILABLE
    parent_count_histogram.render(cr, bounds);
#endif
}
