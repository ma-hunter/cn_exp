#include <iostream>

#include "definitions.h"
#include "utils.h"
#include "handlers.h"
#include "helpers.h"

int main() {
    std::cout << "Hello, World! Hello NpCap!" << std::endl;

    pcap_if_t *all_devs;
    pcap_if_t *d;
    int i_num;
    int i = 0;
    int pkt_num;
    pcap_t *ad_handle;
    char err_buf[PCAP_ERRBUF_SIZE];
    u_int netmask = 0xffffff;;
    bpf_program f_code{};
    if (pcap_findalldevs(&all_devs, err_buf) == -1) {
        fprintf(stderr, "Error in pcap_findalldevs: %s\n", err_buf);
        exit(1);
    }
    for (d = all_devs; d; d = d->next) {
        cout << ++i << "." << d->name;
        if (d->description)
            cout << d->description << endl;
        else cout << " (No description available)" << endl;
    }
    if (i == 0) {
        cout << "\nNo interfaces found! Make sure WinPcap is installed." << endl;
        return -1;
    }
    cout << "Enter the interface number (1-" << i << "): ";
    cin >> i_num;
    if (i_num < 1 || i_num > i) {
        cout << "\nInterface number out of range." << endl;
        pcap_freealldevs(all_devs);
        return -1;
    }
    for (d = all_devs, i = 0; i < i_num - 1; d = d->next, i++);
    if ((ad_handle = pcap_open_live(d->name, 65536, 1, 1000, err_buf)) == nullptr) {
        fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
        pcap_freealldevs(all_devs);
        return -1;
    }
    cout << "listening on " << d->description << "...." << endl;
    pcap_freealldevs(all_devs);
    if (pcap_compile(ad_handle, &f_code, "ip or arp", 1, netmask) < 0) {
        fprintf(stderr, "\nUnable to compile the packet filter. Check the syntax.\n");
        pcap_close(ad_handle);
        return -1;
    }
    if (pcap_setfilter(ad_handle, &f_code) < 0) {
        fprintf(stderr, "\nError setting the filter.\n");
        pcap_close(ad_handle);
        return -1;
    }
    cout << "Please input the num of packets you want to catch(0 for keeping catching): ";
    cin >> pkt_num;
    cout << endl;
    pcap_loop(ad_handle, pkt_num, GET_PKG_HANDLER(packet), nullptr);
    pcap_close(ad_handle);
    cout << endl;
    cout << "Flow counts: " << endl;
    helpers::output_ip_counts();
    cout << endl;
    return 0;
}
