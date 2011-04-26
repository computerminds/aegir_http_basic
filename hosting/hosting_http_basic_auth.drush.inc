<?php

/**
 * Implementation of hook_hosting_site_context_options().
 */
function hosting_http_basic_auth_hosting_site_context_options(&$task) {
  if (isset($task->ref->http_basic_auth_username) && !empty($task->ref->http_basic_auth_username)) {
    $task->context_options['http_basic_auth_username'] = $task->ref->http_basic_auth_username;
    $task->context_options['http_basic_auth_password'] = $task->ref->http_basic_auth_password;
    $task->context_options['http_basic_auth_message'] = $task->ref->http_basic_auth_message;
  }
  else {
    $task->context_options['http_basic_auth_username'] = 'null';
    $task->context_options['http_basic_auth_password'] = 'null';
    $task->context_options['http_basic_auth_message'] = 'null';
  }
}

/**
 * Implementation of hook_drush_context_import().
 */
function hosting_http_basic_auth_drush_context_import($context, &$node) {
  if ($context->type == 'site') {
    if (isset($context->http_basic_auth_username) && !empty($context->http_basic_auth_username)) {
      $node->http_basic_auth_username = $context->http_basic_auth_username;
      $node->http_basic_auth_password = $context->http_basic_auth_password;
      $node->http_basic_auth_message = $context->http_basic_auth_message;
    }
  }

}
