<?php

interface DeployWorker {

  function __construct(DeployFetcher $fetcher);

  function deploy(DeployEndpoint $endpoint);

}
