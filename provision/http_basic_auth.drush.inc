<?php

//include_once(dirname(__FILE__) . '/../provision.service.inc');

/**
 * Implements hook_drush_init()
 */
function http_basic_auth_drush_init(){
  $list = drush_commandfile_list();
  $provision_drush_inc = $list['provision'];
  include_once($provision_drush_inc);
}

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
class Provision_Service_http_basic_auth extends Provision_Service {
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

function _http_basic_auth_create_file($uri, $data, $user, $pass) {
  $app_dir = realpath($data['http_vhostd_path'] . '/..');
  if (!empty($app_dir)) {
    // Make sure the $app_dir has sufficent permissions, this needs to be
    // readable by the apache workers, but has owner aegir:aegir, so we ensure
    // that others can execute (list) this directory.
    // Note that this is not recursive, and the sub directories should keep
    // prying eyes from things they shouldn't be seeing.
    provision_file()->chmod($app_dir, 0771);

    // Compute the path of the password file.
    $path = "{$app_dir}/passwords.d/$uri";

    // Make sure the directory exists.
    provision_file()->create_dir(dirname($path), dt('Passwords'), 0711);

    // Generate the pass using php because nginx server may not have apache tools.
    $pass = crypt($pass);
    $pass_string = "$user:$pass\n";
    // Save the password info to the file.
    provision_file()->file_put_contents($path, $pass_string);

    // Set the permissions:
    provision_file()->chmod($path, 0644);

    // Sync the password out to the server.
    d()->service('http')->sync($path);
  }

  return $path;
}

/*
 * Implementation of hook_provision_apache_vhost_config()
 */
function http_basic_auth_provision_apache_vhost_config($uri, $data) {
  $lines = array();
  $user = d()->http_basic_auth_username;
  $pass = d()->http_basic_auth_password;

  if (!empty($user) && !empty($pass)) {
    // Create the password file.
    $path = _http_basic_auth_create_file($uri, $data, $user, $pass);

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

/*
 * Implementation of hook_provision_nginx_vhost_config()
 */
function http_basic_auth_provision_nginx_vhost_config($uri, $data) {
  $lines = array();
  $user = d()->http_basic_auth_username;
  $pass = d()->http_basic_auth_password;

  if (!empty($user) && !empty($pass)) {
    // Create the password file.
    $path = _http_basic_auth_create_file($uri, $data, $user, $pass);

    // Now add the file to the vhost:
    $root = d()->root;
    $message = !empty(d()->http_basic_auth_message) ? d()->http_basic_auth_message : dt('Restricted access');
    $lines[] = "auth_basic \"$message\";";
    $lines[] = "auth_basic_user_file $path;";
    $lines[] = "";
  }

  return implode("\n", $lines);
}
