ViewDown
========

`ViewDown` is a simple markdown viewer based on Qt5 and QtWebKit.

![README](readme.png)

	$ viewdown -h
	Usage: viewdown [options] file [watch...]
	Markdown viewer, reloading on changes.

	Options:
	  -s <css>       css file to use as stylesheet.
	  -h, --help     Displays this help.
	  -v, --version  Displays version information.

	Arguments:
	  file           markdown file to view and reload on change.
	  watch          files or directories to watch as trigger for reload.

Features
--------

* autoreload on file change
* open links in external browser
* default github style css
* watch additional files / directories as reload trigger

| Key | Action         |
|:---:|:--------------:|
|  q  | close Window   |
|  r  | reload         |
|  o  | open file      |
|  i  | open inspector |

Dependencies
------------

* Qt5
* QtWebKit
* hoedown (<https://github.com/hoedown/hoedown>)

Install
-------

	make

License
-------

MIT License

`github.css` based on <https://github.com/sindresorhus/github-markdown-css>.

