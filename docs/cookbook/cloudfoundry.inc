
.. _cloudfoundry-cookbook:

Cloud Foundry Recipes
---------------------

Recipes for working effectively with a Cloud Foundry environment.

How-To Deploy an App
....................

The Python SDK does not provide this feature.  You should just use ``cf push``.

How-To Authenticate to Cloud Foundry
....................................

The Python SDK does not provide this feature.  You should just use ``cf
login``.

How-To Specify a Python Buildpack
.................................

You can run ``cf buildpacks`` to see the version of the buildpack installed in
Predix, but sometimes you want to use a newer version of the Python buildpack
for a new feature or bug fix.  You can do so by specifying ``-b
https://github.com/cloudfoundry/python-buildpack.git#v1.5.20`` on the command
line or put it in your manifest.yml.

See a post on the Predix Developer Forums for more details:

https://forum.predix.io/articles/24714/how-to-specify-a-python-buildpack.html

How-To Include Private Python Libraries in CF
.............................................

If you have Python libraries in a private source code repository or cannot
publish to public indexes like PyPI you may not know how to include them in
your Cloud Foundry App.

See a how-to post on the Predix Developer Forums for more details:
https://forum.predix.io/articles/24733/how-to-include-your-own-private-python-libraries-i.html

How-To Specify a Different Version of Python
............................................

If you add a ``runtime.txt`` file in the same directory as your manifest.yml
you can specify a single line in the file to signify the version.

For example:

::

    python-3.6.1

or

::

    python-2.7.13

Only specific versions supported by each python_buildpack release so check the
release notes for details.

https://github.com/cloudfoundry/python-buildpack/releases/tag/v1.5.20

See a post on the Predix Developer Forums for more details:
https://forum.predix.io/articles/24718/how-to-specify-a-python-version-for-your-app.html

How-To Delete a Service
.......................

If you used PredixPy to create your service you may be surprised that running
``cf delete-service`` returns an error: *Cannot delete service instance,
service keys and bindings must first be deleted.*

You can use PredixPy to delete services

::

    import predix.admin.app

    admin = predix.admin.app.Manifest()
    service_name = 'my-service'

    if service_name in admin.space.get_instances():
        admin.services.purge(service_name)

    print(admin.space.get_instances())

This can be done with the ``cf`` cli as well of course.  You just run ``cf
service-keys`` to see the named key and then ``cf delete-service-key``.

How-To Create and Delete Spaces
...............................

To test your Python apps, it may be convenient to be able to create and delete
the spaces you are working in.  You can automate this with bash scripts and the
``cf`` cli, or there are some utility methods.

::

    import predix.admin.cf.spaces

    # Create a randomly named space and delete it

    space = predix.admin.cf.spaces.create_temp_space()
    # Do stuff
    space.delete_space()

    # If you want to specify your own space name

    space = predix.admin.cf.spaces.Space()
    space.create_space('my-space-name')
    # Do stuff
    space.delete_space('my-space-name')

How-To Test Apps Without Pushing to Predix Cloud
................................................

You can test your application with Python SimpleHTTPServer which allows you
to visit http://localhost:4000/ to test your app before you do a ``cf push``.

::

    python -m SimpleHTTPServer

You wouldn't use this in production, but it is suitable for quick development
testing.  If used in combination with ``predix.app.Manifest()`` many of the
service bindings will behave similar to how they will when deployed to Cloud
Foundry.  There are a few exceptions such as **PostgreSQL** and **Blobstore**
that are only available when running in a Predix Cloud environment.

How-To Choose Org and Space
...........................

If you run ``cf target`` you can see the org and space that will be used for
any SDK admin operations.  You can use the ``cf target -s <other-space>`` to
use a different space.

If you want to automatically target the same org and space when you are using a
manifest you can bake this into the manifest.yml itself.  The following
demonstrates how to do this:

::

    import predix.admin.app
    app = predix.admin.app.Manifest()
    app.lock_to_org_space()

From then on, any admin operations will focus on the same org and space from
this manifest despite what ``cf target`` currently indicates.

How-To Do Common Cloud Foundry CLI Commands
...........................................

In the following table the ``admin`` object is created with the following
snippet of code:

::

    import predix.admin.app
    admin = predix.admin.app.Manifest()

You can then programmatically with Python do a few of the common things you
might see others do on the command line.  These are mostly for convenience
as you might want to script tools that can handle data from these commands.

+-----------------------+------------------------------------+
|  CF Command           |  PredixPy                          |
+=======================+====================================+
| cf spaces             |  admin.space.get_spaces()          |
+-----------------------+------------------------------------+
| cf services           |  admin.space.get_instances()       |
+-----------------------+------------------------------------+
| cf apps               |  admin.space.get_apps()            |
+-----------------------+------------------------------------+
| cf marketplace        |  admin.space.get_services()        |
+-----------------------+------------------------------------+
| cf create-space       |  admin.space.create_space()        |
+-----------------------+------------------------------------+
| cf org-users          |  admin.space.org.get_users()       |
+-----------------------+------------------------------------+
| cf delete-service -f  |  admin.services.purge(service_name)|
+-----------------------+------------------------------------+

.. _gotty-setup:

How-To Setup GoTTY
..................

GoTTY is an open-source, pre-built application you can run in the Predix Cloud
that provides a bash shell accessible via a web browser.  By using
`predix-gotty-util`_ you can quickly push an app that has ``psql``,
``redis-cli`` and some other utilities available.

Here's what you need to do::

    git clone https://github.com/j12y/predix-gotty-util
    cd predix-gotty-util
    cp manifest.template.yml manifest.yml

Then modify the username and password in the manifest.yml to be something only
you know.  At that point, you can simply ``cf push`` your app and then go to
the resulting URL.  You will need need to ``cf enable-diego gotty`` if you are
still running DEA.

_`predix-gotty-util`: https://github.com/j12y/predix-gotty-util

