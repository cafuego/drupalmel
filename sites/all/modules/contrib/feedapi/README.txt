; $Id: README.txt,v 1.1.2.6.2.9 2009/09/24 23:07:37 alexb Exp $

Purpose and features
--------------------

FeedAPI aggregates feeds on a Drupal website by generating nodes from feeds. 
It provides a straightforward configuration for most use cases and is 
extensible through an API.

* RSS, Atom and RDF feed support
* CSV, ical, KML parsing is available with 3rd party modules
* Drush integration
* Views integration
* OG integration
* Store data from feeds in CCK fields (Feed Element Mapper)
  Read INSTALL.txt about how to get started using the module.

Developers
----------

Take a look on the API documentation if you want to develop for FeedAPI: 
feedapi.api.php

Performance
-----------

Parser Common Syndication is generally faster than Parser SimplePie because it 
uses PHP5-only SimpleXML library.

For large-scale deployments or with special needs, consider refreshing your feeds 
with Drush. Running cron with drush (drush cron) uses significantly less memory, 
in some cases refreshing single feeds with drush can be benefitial 
(drush feedapi refresh [node nid]).

Known issues
------------

* Parser SimplePie submodule may be incompatible with other modules which include
  SimplePie (for example Activity Stream)
* Do not use FeedAPI and poormanscron together (http://drupal.org/node/235328)
* Grant "use local files as feeds" permission when it's non-avoidable. Use it only when you know exactly what you want to do.

Author/credits
--------------

* The maintainers are Áron Novák (http://drupal.org/user/61864) and Alexander Barth (http://drupal.org/user/53995).
  I got lots of useful responses from my mentors
  Ken Rickard, Károly Négyesi (chx), David Norman (deekayen) and Alex Barth.

Sponsored by Development Seed
Started as a Summer of Code 2007 project.
