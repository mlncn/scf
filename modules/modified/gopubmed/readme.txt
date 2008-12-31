How to install:

1) Copy gopubmed directory to your modules directory
2) Enable the module at: admin/build/modules
3) Assign permissions to user roles at: admin/user/access
4) Add GoPubMed search term at: admin/settings/gopubmed/add
5) Now when running cron, gopubmed module will automatically create bibtext file with gopubmed search term and import it automatically into biblio module which will create biblio node.