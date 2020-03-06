// #include <linux/kernel.h>
// #include <linux/module.h>
// #include <linux/netfilter.h>
// #include <linux/netfilter_ipv4.h>
// #include <linux/ip.h>
// #include <linux/tcp.h>

// MODULE_LICENSE("GPL");

// static struct nf_hook_ops nfho;

// unsigned int hook_funcion(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
// {
//     register struct tcphdr *tcph;
//     register struct iphdr *iph;
    
//     struct iphdr *ip_header = (struct iphdr *)skb_network_header(skb); //you can access to IP source and dest - ip_header->saddr, ip_header->daddr
//     struct tcphdr *tcp_header;
//     if (ip_header->protocol == 6) //TCP protocol
//     {
//         printk(KERN_INFO "TCP Packet\n");
//         tcp_header = (struct tcphdr *)(skb_transport_header(skb)+20); //Note: +20 is only for incoming packets
//         printk(KERN_INFO "Source Port: %u\n", tcp_header->source); //can access dest in the same way
//     }
//     return NF_ACCEPT;

//     // check if it is TCP package here
//     if(skb == 0)
//         return NF_ACCEPT;
//     iph = ip_hdr(skb);
//     if(iph->protocol != IPPROTO_TCP)
//         return NF_ACCEPT;
//     tcph = tcp_hdr(skb);

//     // debug here
//     printk("tcph->dest = %d", ntohs(tcph->dest));
//     if(tcph->dest != 80)
//         return NF_ACCEPT;

//     return NF_ACCEPT;
// }

// // unsigned int hook_func(unsigned int hooknum, struct sk_buff *skb, const struct net_device *in, 
// // const struct net_device *out, int (*okfn)(struct sk_buff *))
// // {
// //   struct iphdr *ip_header = (struct iphdr *)skb_network_header(skb); //you can access to IP source and dest - ip_header->saddr, ip_header->daddr
// //   struct tcphdr *tcp_header;
// //   if (ip_header->protocol == 6) //TCP protocol
// //   {
// //     printk(KERN_INFO "TCP Packet\n");
// //     tcp_header = (struct tcphdr *)(skb_transport_header(skb)+20); //Note: +20 is only for incoming packets
// //     printk(KERN_INFO "Source Port: %u\n", tcp_header->source); //can access dest in the same way
// //   }
// //   return NF_ACCEPT;                                                                   //accept the packet
// // }

// static int __init hook_init(void)
// {
//     int ret = 0;
//     struct net *n;

//     nfho.hook = hook_funcion;
//     nfho.pf = NFPROTO_IPV4;
//     nfho.hooknum = NF_INET_FORWARD;
//     nfho.priority = NF_IP_PRI_MANGLE;
//     for_each_net(n)
//         ret += nf_register_net_hook(n, &nfho);

//     printk("challenge start\n");
//     printk("nf_register_hook returnd %d\n", ret);

//     return 0;
// }

// static void __exit hook_exit(void)
// {
//     struct net *n;

//     for_each_net(n)
//         nf_unregister_net_hook(n, &nfho);

//     printk("challenge stop\n");
// }

// module_init(hook_init);
// module_exit(hook_exit);

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/ip.h>
#include <linux/netfilter.h>

//#undef __KERNEL__
#include <linux/netfilter_ipv4.h>
//#define __KERNEL__


// I am using this code on my VM. Was interested in capturing only non-bridge packets
const __be32 HOST_IP_ADDR = 20687040;
const __be32 LOCAL_IP_ADDR = 171681984;

static struct nf_hook_ops nfin;
static struct nf_hook_ops nfout;


// In every netfilter guid you will find Linux kernel 2.x version of netfilter:
//typedef unsigned int nf_hookfn(unsigned int hooknum,
//            		               struct sk_buff *skb,
//                               const struct net_device *in,
//                               const struct net_device *out,
//                               int (*okfn)(struct sk_buff *))
//{

// In 4.3- it was look like this:
//typedef unsigned int nf_hookfn(const struct nf_hook_ops *ops,
//                               struct sk_buff *skb,
//                               const struct nf_hook_state *state);

// Since 4.4+ it was generalised:
static unsigned int hook_func_in(void *priv,
                                 struct sk_buff *skb,
                                 const struct nf_hook_state *state)
{
    struct ethhdr *eth;
    struct iphdr *ip_header;

    eth = (struct ethhdr*)skb_mac_header(skb);
    ip_header = (struct iphdr *)skb_network_header(skb);

    if (HOST_IP_ADDR == ip_header->saddr)
          return NF_ACCEPT;

    printk("NF_IP_LOCAL_IN hook:\n");
    printk("src mac %pM, dst mac %pM\n", eth->h_source, eth->h_dest);
    printk("src IP addr:=%pI4\n", &ip_header->saddr);
    return NF_ACCEPT;
}

static unsigned int hook_func_out(void *priv,
                                 struct sk_buff *skb,
                                 const struct nf_hook_state *state)
{
    struct ethhdr *eth;
    struct iphdr *ip_header;

    eth = (struct ethhdr*)skb_mac_header(skb);
    ip_header = (struct iphdr *)skb_network_header(skb);

    if (LOCAL_IP_ADDR == ip_header->saddr)
          return NF_ACCEPT;

    printk("NF_IP_LOCAL_OUT hook:\n");
    printk("src mac %pM, dst mac %pM\n", eth->h_source, eth->h_dest);
    printk("src IP addr:=%pI4\n", &ip_header->saddr);
    return NF_ACCEPT;
}

static int __init init_main(void)
{
    struct net *n;
    nfin.hook     = hook_func_in;
    nfin.hooknum  = 1; // NF_IP_LOCAL_IN // TODO
    nfin.pf       = PF_INET;
    nfin.priority = NF_IP_PRI_FIRST;
    for_each_net(n) {
        nf_register_net_hook(n, &nfin);
    }
    // nf_register_hook(&nfin);

    nfout.hook     = hook_func_out;
    nfout.hooknum  = 3; // NF_IP_LOCAL_OUT // TODO
    nfout.pf       = PF_INET;
    nfout.priority = NF_IP_PRI_FIRST;
    for_each_net(n) {
        nf_register_net_hook(n, &nfout);
    }
    

    return 0;
}



static void __exit cleanup_main(void)
{
    struct net *n1;
    for_each_net(n1) {
        nf_unregister_net_hook(n1, &nfin);
        nf_unregister_net_hook(n1, &nfout);
    }

}
module_init(init_main);
module_exit(cleanup_main);