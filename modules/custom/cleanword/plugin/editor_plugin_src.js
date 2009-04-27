/**
 * $Id$
 *
 * @author Agaric
 * @copyright Copyright © 2009, Agaric Design Collective.
 */

(function() {
	var DOM = tinymce.DOM, Event = tinymce.dom.Event, each = tinymce.each, is = tinymce.is;
	
	// Load plugin specific language pack
	tinymce.PluginManager.requireLangPack('cleanword');

	tinymce.create('tinymce.plugins.CleanwordPlugin', {
		/**
		 * Returns information about the plugin as a name/value array.
		 * The current keys are longname, author, authorurl, infourl and version.
		 *
		 * @return {Object} Name/value array containing information about the plugin.
		 */
		getInfo : function() {
			return {
				longname : 'Cleanword plugin',
				author : 'Benjamin, Agaric Design Collective',
				authorurl : 'http://agaricdesign.com',
				infourl : 'http://agaricdesign.com/note/creating-word-paste-plugin-for-tinymce',
				version : "1.0"
			};
		},
		
		/**
		 * Initializes the plugin, this will be executed after the plugin has been created.
		 * This call is done before the editor instance has finished it's initialization so use the onInit event
		 * of the editor instance to intercept that event.
		 *
		 * @param {tinymce.Editor} ed Editor instance that the plugin is initialized in.
		 * @param {string} url Absolute URL to where the plugin is located.
		 */
    init : function(ed, url) {
		  var t = this;
		  t.editor = ed;
			// Register the command so that it can be invoked by using tinyMCE.activeEditor.execCommand('mceExample');
			ed.addCommand('mceCleanword', function() {
        var s = ed.selection, b = s.getBookmark();
        ed.execCommand('selectall');
//        ed.execCommand('mcePasteWord',false,ed.getContent());
					t._insertCleanWordContent(ed.getContent());
        s.moveToBookmark(b);
			});
			
			// Register button
			ed.addButton('cleanword', {
				title : 'cleanword.desc',
				cmd : 'mceCleanword',
				image : url + '/images/cleanword.gif'
			});

      ed.onEvent.add(function(ed, l) {
          if (!this.last_count) {
            this.last_count = 0;
          }
          if ((ed.dom.doc.body.innerHTML.length - this.last_count) > 25) {
//            alert(this.last_count + ' ' + ed.dom.doc.body.innerHTML.length);
            ed.execCommand('mceCleanword');          
          }
          this.last_count = ed.dom.doc.body.innerHTML.length;
      });
    }, 
    // private methods
      
    // modeled on paste plugin's _insertWordContent method
		_insertCleanWordContent : function(content) { 
			var t = this, ed = t.editor;

			if (content && content.length > 0) {
				// Cleanup Word content
				var bull = String.fromCharCode(8226);
				var middot = String.fromCharCode(183);

				if (ed.getParam('paste_insert_word_content_callback'))
					content = ed.execCallback('paste_insert_word_content_callback', 'before', content);

				var rl = ed.getParam("paste_replace_list", '\u2122,<sup>TM</sup>,\u2026,...,\x93|\x94|\u201c|\u201d,",\x60|\x91|\x92|\u2018|\u2019,\',\u2013|\u2014|\u2015|\u2212,-').split(',');
				for (var i=0; i<rl.length; i+=2)
					content = content.replace(new RegExp(rl[i], 'gi'), rl[i+1]);

				if (this.editor.getParam("paste_convert_headers_to_strong", false)) {
					content = content.replace(new RegExp('<p class=MsoHeading.*?>(.*?)<\/p>', 'gi'), '<p><b>$1</b></p>');
				}

				content = content.replace(new RegExp('tab-stops: list [0-9]+.0pt">', 'gi'), '">' + "--list--");
				content = content.replace(new RegExp(bull + "(.*?)<BR>", "gi"), "<p>" + middot + "$1</p>");
				content = content.replace(new RegExp('<SPAN style="mso-list: Ignore">', 'gi'), "<span>" + bull); // Covert to bull list
				content = content.replace(/<o:p><\/o:p>/gi, "");
				content = content.replace(new RegExp('<br style="page-break-before: always;.*>', 'gi'), '-- page break --'); // Replace pagebreaks
				content = content.replace(/<!--([\s\S]*?)-->|<style>[\s\S]*?<\/style>/g, "");  // Word comments
				content = content.replace(/<(meta|link)[^>]+>/g, ""); // Header elements

				if (this.editor.getParam("paste_remove_spans", true))
					content = content.replace(/<\/?span[^>]*>/gi, "");

				if (this.editor.getParam("paste_remove_styles", true))
					content = content.replace(new RegExp('<(\\w[^>]*) style="([^"]*)"([^>]*)', 'gi'), "<$1$3");

				content = content.replace(/<\/?font[^>]*>/gi, "");

				// Strips class attributes.
				switch (this.editor.getParam("paste_strip_class_attributes", "all")) {
					case "all":
						content = content.replace(/<(\w[^>]*) class=([^ |>]*)([^>]*)/gi, "<$1$3");
						break;

					case "mso":
						content = content.replace(new RegExp('<(\\w[^>]*) class="?mso([^ |>]*)([^>]*)', 'gi'), "<$1$3");
						break;
				}

				content = content.replace(new RegExp('href="?' + this._reEscape("" + document.location) + '', 'gi'), 'href="' + this.editor.documentBaseURI.getURI());
				content = content.replace(/<(\w[^>]*) lang=([^ |>]*)([^>]*)/gi, "<$1$3");
				content = content.replace(/<\\?\?xml[^>]*>/gi, "");
				content = content.replace(/<\/?\w+:[^>]*>/gi, "");
				content = content.replace(/-- page break --\s*<p>&nbsp;<\/p>/gi, ""); // Remove pagebreaks
				content = content.replace(/-- page break --/gi, ""); // Remove pagebreaks

		//		content = content.replace(/\/?&nbsp;*/gi, ""); &nbsp;
		//		content = content.replace(/<p>&nbsp;<\/p>/gi, '');

				if (!this.editor.getParam('force_p_newlines')) {
					content = content.replace('', '' ,'gi');
					content = content.replace('</p>', '<br /><br />' ,'gi');
				}

				if (!tinymce.isIE && !this.editor.getParam('force_p_newlines')) {
					content = content.replace(/<\/?p[^>]*>/gi, "");
				}

				content = content.replace(/<\/?div[^>]*>/gi, "");

				// Convert all middlot lists to UL lists
				if (this.editor.getParam("paste_convert_middot_lists", true)) {
					var div = ed.dom.create("div", null, content);

					// Convert all middot paragraphs to li elements
					var className = this.editor.getParam("paste_unindented_list_class", "unIndentedList");

					while (this._convertMiddots(div, "--list--")) ; // bull
					while (this._convertMiddots(div, middot, className)) ; // Middot
					while (this._convertMiddots(div, bull)) ; // bull

					content = div.innerHTML;
				}

				// Replace all headers with strong and fix some other issues
				if (this.editor.getParam("paste_convert_headers_to_strong", false)) {
					content = content.replace(/<h[1-6]>&nbsp;<\/h[1-6]>/gi, '<p>&nbsp;&nbsp;</p>');
					content = content.replace(/<h[1-6]>/gi, '<p><b>');
					content = content.replace(/<\/h[1-6]>/gi, '</b></p>');
					content = content.replace(/<b>&nbsp;<\/b>/gi, '<b>&nbsp;&nbsp;</b>');
					content = content.replace(/^(&nbsp;)*/gi, '');
				}

				content = content.replace(/--list--/gi, ""); // Remove --list--

				if (ed.getParam('paste_insert_word_content_callback'))
					content = ed.execCallback('paste_insert_word_content_callback', 'after', content);

				// Insert cleaned content
				this.editor.execCommand("mceInsertContent", false, content);

				if (this.editor.getParam('paste_force_cleanup_wordpaste', true)) {
					var ed = this.editor;

					window.setTimeout(function() {
						ed.execCommand("mceCleanup");
					}, 1); // Do normal cleanup detached from this thread
				}
			}      
		},
		
		_reEscape : function(s) {
			var l = "?.\\*[](){}+^$:";
			var o = "";

			for (var i=0; i<s.length; i++) {
				var c = s.charAt(i);

				if (l.indexOf(c) != -1)
					o += '\\' + c;
				else
					o += c;
			}

			return o;
		},

		_convertMiddots : function(div, search, class_name) {
			var ed = this.editor, mdot = String.fromCharCode(183), bull = String.fromCharCode(8226);
			var nodes, prevul, i, p, ul, li, np, cp, li;

			nodes = div.getElementsByTagName("p");
			for (i=0; i<nodes.length; i++) {
				p = nodes[i];

				// Is middot
				if (p.innerHTML.indexOf(search) == 0) {
					ul = ed.dom.create("ul");

					if (class_name)
						ul.className = class_name;

					// Add the first one
					li = ed.dom.create("li");
					li.innerHTML = p.innerHTML.replace(new RegExp('' + mdot + '|' + bull + '|--list--|&nbsp;', "gi"), '');
					ul.appendChild(li);

					// Add the rest
					np = p.nextSibling;
					while (np) {
						// If the node is whitespace, then
						// ignore it and continue on.
						if (np.nodeType == 3 && new RegExp('^\\s$', 'm').test(np.nodeValue)) {
								np = np.nextSibling;
								continue;
						}

						if (search == mdot) {
								if (np.nodeType == 1 && new RegExp('^o(\\s+|&nbsp;)').test(np.innerHTML)) {
										// Second level of nesting
										if (!prevul) {
												prevul = ul;
												ul = ed.dom.create("ul");
												prevul.appendChild(ul);
										}
										np.innerHTML = np.innerHTML.replace(/^o/, '');
								} else {
										// Pop the stack if we're going back up to the first level
										if (prevul) {
												ul = prevul;
												prevul = null;
										}
										// Not element or middot paragraph
										if (np.nodeType != 1 || np.innerHTML.indexOf(search) != 0)
												break;
								}
						} else {
								// Not element or middot paragraph
								if (np.nodeType != 1 || np.innerHTML.indexOf(search) != 0)
										break;
							}

						cp = np.nextSibling;
						li = ed.dom.create("li");
						li.innerHTML = np.innerHTML.replace(new RegExp('' + mdot + '|' + bull + '|--list--|&nbsp;', "gi"), '');
						np.parentNode.removeChild(np);
						ul.appendChild(li);
						np = cp;
					}

					p.parentNode.replaceChild(ul, p);

					return true;
				}
			}

			return false;
		}
	});
	
	// Register plugin
	tinymce.PluginManager.add('cleanword', tinymce.plugins.CleanwordPlugin);
})();
