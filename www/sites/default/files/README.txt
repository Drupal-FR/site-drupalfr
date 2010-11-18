$Id: README.txt,v 1.5.2.3 2008/01/07 20:58:55 kbahey Exp $

Copyright 2005-2007 http://2bits.com

Description
-----------
This module provides your sites visitors to browse and search jobs, or post their
resumes for employers/recruiters to see. It also allows job seekers to apply for
jobs. In effect, your site can be a mini monster.com or hotjobs.com.

It can be setup for one single firm (e.g. a recruiting company), or as a general
job site (e.g. like monster.com).

Jobs and resumes (CVs) are two different types of content. They can be a simple
type (no extra fields) or you can use CCK to create more sophisticated content
types (e.g. you can add whether the job is a full time or part time, permanent
or contract, years of experience, ...etc.).

Resumes can also have attachments (e.g. resume in Word or PDF format).

Of course, jobs can be assigned various categories, such as jobs by location,
jobs by industry, ...etc.

Recruiters can view all jobs in the system.

Site visitors can browse jobs, either by date, or by category (as set above),
or with anything that views can do.

Visitors can register on the site, and then are allowed to post resumes, and
apply for jobs. Using extra modules, you can setup the site so that recruiters/
employers would have to pay to post jobs on the site. You can do the same for
job seekers if you so wish.

When someone applies for a job, they can select from resumes they have created.
If they do not have resumes, they will be prompted to create one.  Once they apply,
and email is sent to the person who posted the job, with a link to the resume, and
the user's profile. The resume will also be placed in a queue for review.

Optionally, resumes are kept private and only a recruiter or the author can see
it. Other job seekers cannot view other people's resumes.

Search functions allow users who can view resumes to search them, and the same
applies to jobs as well.

Expanded usage with other modules
---------------------------------
Using the views module, as well as CCK, you can present the jobs in various ways
to the users, with drop down selectable lists.

You can also use the location module to setup proximity searches for jobs by zip
code, city, or jobs within 25 miles of zip code 90210.

If you want job postings and/or resumes to expire after a preset period, you can
use the scheduler module to do that.

Also, if you want users to pay for posting jobs or resumes, you can use userpoints
with ecommmerce integration, and userpoints node limit.

You can use Ecommerce with EC Role module so users can buy the employer/recruiter
role so they are able to post jobs. Similarly, you can setup job seeker as a 
role, and users can post their resumes and apply for jobs if they buy that role.

Installation and Configuration
------------------------------
Installation
------------
To install this module, upload or copy the job folder and all the files
in it to your modules directory.

Configuration
-------------
To enable this module do the following:

1. Go to Administer -> Site Building -> Modules, and enable Job and Resume.
   Optionally, if you have the Job access and Resume access modules, enable
   them as well.

2. Go to Administer -> Site Configuration -> Job.
   Select which node type(s) would be designated as jobs that can be
   applied for.

3. Go to Administer -> Site Configuration -> Resume.
   Select which node type(s) would be designated as resumes that can be
   used to apply for jobs.

4. Go to Administer -> User Configuration -> Roles.
   Create roles for the job seekers, and the recruiter/employer.

5. Go to Administer -> User Configuration -> Access Control.
   Assign view jobs to the recruiter/employer role, and apply for jobs to the
   job seeker role. Also assign view resume to the recruiter/employer role.

6. If you are using the job access and resume access modules, you have to-
   set the permissions correctly. Visit Administer -> Content Management ->
   Post settings and click on "Rebuild Permissions".

Extending the modules
---------------------
New features tailored to your needs can be added to let you have the functionality
you want.

For example:
- unpublish resumes or jobs automatically after a certain number of days have
  passed, but keep them published for paying members.
- allow a certain number of jobs and/or resumes to be posted for paying customers
  who buy certain points via ecommerce module

Contact the author for details.

Author
------
Khalid Baheyeldin  http://baheyeldin.com of http://2bits.com

The author can also be contacted for paid customizations of this module as well as
Drupal consulting, installation, development, and customizations.
