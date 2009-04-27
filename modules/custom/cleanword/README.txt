Known bugs:

Currently, inline styles are disabled.  This affects current implementations of:
  Indenting.
  Alignment.

We recommend implementing these features without using inline styles rather than allowing inline styles in the document.

When indenting a list, TinyMCE implements this by adding <ol> tags without <li> tags.  Cleanup then inserts <li> tags.
