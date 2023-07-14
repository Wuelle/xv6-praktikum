#include "kernel/net/arp.h"
#include "kernel/net/ip.h"
#include "kernel/net/net.h"

static connection_entry connections[MAX_TRACKED_CONNECTIONS] = {0};
static struct spinlock connections_lock = {0};

void
net_init()
{
    initlock(&connections_lock, "Connection Tracker Lock");
    arp_init();
    ip_init();
}

/**
 * Wait for a response.
 * Releases a held lock upon entering sleep mode to ensure no loss of response
*/
void
wait_for_response(connection_identifier id, void* buf, struct spinlock* lock)
{
    acquire(&connections_lock);
    release(lock);
    for (int i = 0; i < MAX_TRACKED_CONNECTIONS; i++) {
        if (connections[i].signal == 0) {
            connections[i].signal = 1;
            connections[i].identifier = id;
            connections[i].buf = buf;
            // Sleep dinge. Wakeup on response
            // Maybe replace with spin if reply not guaranteed
            sleep(&connections[i].signal, &connections_lock);
            // Reset structure
            connections[i].signal = 0;
            connections[i].identifier.identification.value = 0;
            connections[i].buf = NULL;
            release(&connections_lock);
            return;
        }
    }
    panic("No free slot for waiting for response\n");
}

int
notify_of_response(struct ethernet_header* ethernet_header)
{
    connection_identifier id = compute_identifier(ethernet_header);
    acquire(&connections_lock);
    connection_entry* entry = get_entry_for_identifier(id);

    if (entry != NULL) {
        copy_data_to_entry(entry, ethernet_header);
        // Wakeup
        entry->signal = 2;
        release(&connections_lock);
        wakeup(&entry->signal);
        return 0;
    } else {
        release(&connections_lock);
        return -1;
    }
}

connection_entry*
get_entry_for_identifier(connection_identifier id)
{
    // Loop over array
    for (int i = 0; i < MAX_TRACKED_CONNECTIONS; i++) {
        connection_identifier entry_id = connections[i].identifier;
        if (entry_id.identification.value == id.identification.value && entry_id.protocol == id.protocol) {
            return &connections[i];
        }
    }

    return NULL;
}

void
copy_data_to_entry(connection_entry* entry, struct ethernet_header* ethernet_header)
{
    if (entry == NULL) {
        return;
    }

    uint64 offset = sizeof(struct ethernet_header);
    uint64 length = 0;
    switch(entry->identifier.protocol) {
        case CON_ARP:
            length = sizeof(struct arp_packet);
            break;
        case CON_DHCP:
        case CON_ICMP:
        case CON_TCP:
        case CON_UDP:
            offset += sizeof(struct ipv4_header);
            struct ipv4_header* ipv4_header = (struct ipv4_header*) (uint8*) ethernet_header + sizeof(ethernet_header);
            length = ipv4_header->total_length - ipv4_header->header_length;
            break;

        default:
            return;
    }
    uint8* data = (uint8*) ethernet_header;

    memmove(entry->buf, data, length);
}

connection_identifier 
compute_identifier(struct ethernet_header* ethernet_header)
{
    connection_identifier id = {0};
    memreverse(&ethernet_header->type, sizeof(ethernet_header->type));
    switch (ethernet_header->type)
    {
        case ETHERNET_TYPE_ARP:
            struct arp_packet* arp_packet = (struct arp_packet*) ( (uint8*) ethernet_header + sizeof(struct ethernet_header));
            memmove(id.identification.arp.ip_addr, arp_packet->ip_src, IP_ADDR_SIZE);
            id.protocol = CON_ARP;
            break;
        case ETHERNET_TYPE_IPv4:
            struct ipv4_header* ipv4_header = (struct ipv4_header*) ( (uint8*) ethernet_header + sizeof(struct ethernet_header));
            switch (ipv4_header->protocol)
            {
                case IP_PROT_ICMP:
                    id.protocol = CON_ICMP;
                    pr_notice("Not supported: Dropping ICMP\n");
                    break;
                case IP_PROT_TCP:
                    id.protocol = CON_TCP;
                    pr_notice("Not supported: Dropping TCP\n");
                    break;
                case IP_PROT_UDP:
                    id.protocol = CON_UDP;
                    pr_notice("Not supported: Dropping UDP\n");
                    break;
                default:
                    id.protocol = CON_UNKNOWN;
                    pr_notice("Unknown IP-Protocol field: Dropping\n");
                    break;
            }
            break;
        // Anything unknown, including regular ethernet
        default: 
            pr_notice("Dropping packet with type: %x\n", ethernet_header->type);
            break;

    }
    return id;
}