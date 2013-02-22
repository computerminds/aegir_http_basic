Aegir HTTP basic authentication
===============================

Introduction
------------

This is a simple module and drush script for Aegir that allows you to specify a
username and password for HTTP basic authentication per site in Aegir.

If you don't know what Aegir is, you'll probably want to start there and come
back when you really know that you want to use this code.
http://aegirproject.org/

Installation
------------

There are two parts to the code:
- A Drupal module for hostmaster - contained in the /hosting directory. Install
  this like any other Drupal module into you hostmaster site.
- A provision Drush script - contained in the /provision directory. Copy this
  into /var/aegir/.drush/aegir_http_basic/ on your Aegir master
   server.
- Aegir sometimes struggles to set the correct permissions on some directories
  that might stop your site from working. Make sure that
  `/var/aegir/config/server_NAME/apache` is executable by 'others', which just
  means that the directory is browsable

Now just enable the module in the Aegir frontend, and you're ready to go.


Usage
-----

When creating or editing a site, you can optionally add a username, password and
message for the HTTP basic authentication. Leaving this blank will do nothing,
but if they are filled in then those credentials will be sent to the backend and
required to access the site.

Caveats
-------

This module is mostly a demonstration, so it may not work.
Also, the passwords you enter are popped onto the command line of the master
server, so you should not use any secure passwords for this. Also, HTTP Basic
authentication is sent over the internet as plain text, so you really shouldn't
be using passwords you want to keep secret.
This module is just to stop most people from accessing a site.
