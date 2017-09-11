/**
*@file 
*@brief settings and definitions basic functions
*@author M.Ulbricht 2015
**/
int INR_CTL_init_proc (struct pci_dev *dev);
int INR_CTL_remove_proc (struct pci_dev *dev);
void INR_CTL_init_ovs (void);
void INR_CTL_ovs_d (void *nix);
void INR_CTL_ovs_flow_flush (void);
void INR_CTL_ovs_flow_rm (void);
void INR_CTL_ovs_flow_add (void);
#define INR_PCI_BAR1_read(addr)		readl(gBaseVirt1+addr)	/*read from bar0 */
#define INR_PCI_BAR1_write(content, addr)	writel(content, (gBaseVirt1 + addr))	/*write to bar0 */

enum INR_TN_ovsctl_cmd_type
{
  INR_TN_ovsctl_cmd_ADD,
  INR_TN_ovsctl_cmd_RM,
  INR_TN_ovsctl_cmd_FLUSH
};
