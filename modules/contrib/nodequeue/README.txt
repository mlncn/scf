The Node Queue module allows an administrator to arbitrarily put nodes in a
group for some purpose; examples of this might be to highlight one particular
node, as in a typical news site's Lead Article. Another use might be to create
a block listing teasers for 5 forum posts that the admin thinks are important.
Another use might be to create a group of nodes, and then have a block or the
front page offer one of these randomly.

Queues can be set to allow only certain types of nodes to be added to the
queue. Queue can be a fixed size or of infinite length. And the admin can
select which roles have permission to add nodes to a given queue.

Once a queue is set up, a new tab will appear on eligible nodes for eligible
users. This tab will allow the user--regardless of edit permissions--to add or
remove that node from the queue. Queue admins can view the nodes in the queue,
and can modify the order of items already in the queue. Items may also appear
in a nodes links area to add/remove them from the queue.

When a node is added to the queue, it is added to the back of the queue. If a
queue is full when a node is added, the front of the queue is removed. 

It is highly recommended that you use the Views module to display your queues.
However, if you choose not to, here is an alternative:

In order to actually do something useful with a node queue, the admin is
required to use a small PHP snippet. This is a very small snippet, and while
it would have been possible to write code to avoid this, the PHP allows
a much greater flexibility to the admin than to keep it all configured
through menus, though it is somewhat less intuitive.

To Create a Block to Display Node Titles of a Queue
===================================================

You'll need the Queue ID, which is easily extracted from the URL on the
queue administration page.

Create a new block, and insert the following PHP snippet into the block:

   <?php print nodequeue_node_titles(QUEUEID); ?>

If you want this queue to be printed in the reverse order, you can tell it
to print backward:

   <?php print nodequeue_node_titles(QUEUEID, '', true); ?>

The '' in the line above is an optional title field. Feel free to put
something here, but it's not terribly necessary in a block.

To Create a Page to Display Node Teasers of a Queue
===================================================

Like above, you'll need the Queue ID.

Create a new page (or a new dashboard!) or any node type you like, really, 
and set the input filter to PHP. Insert the following PHP snippet:

   <?php print nodequeue_nodes(QUEUEID); ?>

There are a few more options available here; changing the order of the nodes,
whether or not to use teasers or full nodes, whether or not to display the
links, and how much of the queue to display. See below.

To Show Just The First or Last Element of a Queue
=================================================

Starting with the examples above, but use the following:

   <?php print nodequeue_fetch_front(QUEUEID); ?>

or

   <?php print nodequeue_fetch_back(QUEUEID); ?>

Remember that the front of the queue will have the least recently added
nodes (unless it was rearranged manually), and the back will have the
most recently added.

Available Functions and Descriptions
====================================

nodequeue_node_titles($qid, $title = '', $backward = true, $from = 0, $count = 0)
    Display a title list of the queue. If backward is true (the default) the
    list will be from back (newest) to front (oldest).

nodequeue_nodes($qid, $backward = true, $teasers = true, $links = true, 
         $from = 0, $count = 0) 
    Display the nodes of a queue. If backward is true (the default) the
    list will be from back (newest) to front (oldest). If $count
    is set to non-zero, it will use a range. For example, passing 
    $from = 2 and $count = 2 will show the 3rd and 4th elements of
    the queue. ($count starts at 0, not 1.)

    if $teasers is true, the node teaser will be shown; otherwise the full
    node will be shown.

nodequeue_fetch_front($qid, $teasers = true, $links = true) 
    Fetch the node at the front of the queue.

nodequeue_fetch_back($qid, $teasers = true, $links = true) 
    Fetch the node at the back of the queue.

function nodequeue_fetch_random($qid, $teasers = true, $links = true) 
    Fetch a random node from the queue

Actions Module Integration
==========================

The node queue module provides two actions, so that workflow can add and
remove items from queues.



