<?php

include_once(dirname(__FILE__) . '/../provision.service.inc');

/**
 * Expose the service type this extension defines to provision.
 *
 * @return
 *   An array with the service type the key, and the default implementation the value.
 */
function http_basic_auth_provision_services() {
  return array('http_basic_auth' => NULL);
}

/**
 * The subfolder service base class.
 */
class provisionService_http_basic_auth extends provisionService {
  public $service = 'http_basic_auth';

  /**
   * Add the subfolder_path property to the site context.
   */
  static function subscribe_site($context) {
    $context->setProperty('http_basic_auth_username');
    $context->setProperty('http_basic_auth_password');
    $context->setProperty('http_basic_auth_message');
  }
}

/*
 * Implementation of hook_provision_apache_vhost_config()
 */
function http_basic_auth_provision_apache_vhost_config($uri, $data) {
  $lines = array();
  $user = d()->http_basic_auth_username;
  $pass = d()->http_basic_auth_password;

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
    $message = !empty(d()->http_basic_auth_message) ? d()->http_basic_auth_message : dt('Restricted access');
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
