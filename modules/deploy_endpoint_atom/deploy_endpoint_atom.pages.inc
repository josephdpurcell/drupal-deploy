<?php

/**
 * Page callback for the atom feed.
 */
function deploy_endpoint_atom_page(DeployPlan $plan, DeployEndpoint $endpoint) {
  $plan->deploy($endpoint->name);
  die();
}
