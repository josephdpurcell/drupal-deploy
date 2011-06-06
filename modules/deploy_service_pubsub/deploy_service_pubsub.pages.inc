<?php

/**
 * Page callback for the PubSubHubBub feed.
 */
function deploy_service_pubsub_page(DeployPlan $plan) {
  $plan->deploy();
  die();
}
