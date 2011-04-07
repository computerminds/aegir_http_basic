<?php


/**
 * @file
 *   The hosting feature definition for HTTP basic authentication.
 */

/**
 * Register a hosting feature with Aegir.
 *
 * This will be used to generate the 'admin/hosting' page.
 *
 * @return
 *  associative array indexed by feature key.
 */
function hosting_http_basic_auth_hosting_feature() {
  $features['http_basic_auth'] = array(
    // title to display in form
    'title' => t('HTTP Basic Authentication'),
    // description
    'description' => t('Allows admins to specify HTTP basic authentication for sites.'),
    // initial status ( HOSTING_FEATURE_DISABLED, HOSTING_FEATURE_ENABLED, HOSTING_FEATURE_REQUIRED )
    'status' => HOSTING_FEATURE_DISABLED,
    // module to enable/disable alongside feature
    'module' => 'hosting_http_basic_auth',
    // which group to display in ( null , experimental )
    'group' => 'experimental'
    );
  return $features;
}
