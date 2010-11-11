<?php


/**
 * Implementation of drush_hook_pre_hosting_task()
 */
function drush_hosting_http_basic_auth_pre_hosting_task() {
  $task =& drush_get_context('HOSTING_TASK');
  if ($task->ref->type == 'site' && ($task->task_type == 'install' || $task->task_type == 'verify')) {
    $task->options['http_basic_auth_username'] = $task->ref->http_basic_auth_username;
    $task->options['http_basic_auth_password'] = $task->ref->http_basic_auth_password;
    $task->options['http_basic_auth_message'] = $task->ref->http_basic_auth_message;
  }
}
