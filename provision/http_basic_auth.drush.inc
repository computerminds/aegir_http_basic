<?php

/*
 * Implementation of hook_provision_apache_vhost_config()
 */
function http_basic_auth_provision_apache_vhost_config($uri, $data) {
  $lines = array();
  $user = drush_get_option('http_basic_auth_username', '');
  $pass = drush_get_option('http_basic_auth_password', '');

  if (!empty($user) && !empty($pass)) {

    // Compute the path of the password file.
    $path = $data['http_vhostd_path'] . '/../passwords.d/' . $uri;

    // Make sure the directory exists.
    provision_file()->create_dir(dirname($path), dt('Passwords'), 0711);

    // Save the password info to the file.
    // TODO: don't pop the password on the command line!
    $cmd = 'htpasswd -cb %s %s %s';

    drush_shell_exec($cmd, $path, $user, $pass);
    $path = realpath($path);

    // Set the permissions:
    provision_file()->chmod($path, 0644);

    // Sync the password out to the server.
    d()->service('http')->sync($path);

    // Now add the file to the vhost:
    $root = d()->root;
    $message = drush_get_option('http_basic_auth_message', dt('Restricted access'));
    $lines[] = "  <Directory \"$root\">";
    $lines[] = "    # HTTP Basic Authentication added by Aegir";
    $lines[] = "    AuthType Basic";
    $lines[] = "    AuthName \"$message\"";

    $lines[] = "    AuthUserFile $path";
    $lines[] = "    Require valid-user";
    $lines[] = "  </Directory>";
  }

  return implode("\n", $lines);
}
