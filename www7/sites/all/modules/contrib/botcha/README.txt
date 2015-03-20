
BOTCHA Module
-------------
by Ilya Ivanchenko, iva2k@yahoo.com

Summary
-------

BOTCHA provides an unobtrusive anti-spam protection for website forms. There are 3 key advantages:

+ Unlike challege-response tests like CAPTCHA, human users are not burdened by solving any puzzles.

+ Unlike Mollom or Akismet, form data is not submitted to a 3rd party server for analysis.

+ Protection by BOTCHA is variable, adaptible, scalable, and virtually unlimited


In CAPTCHA, each user is burdened to prove he/she is human by solving a puzzle.
Initially CAPTCHA was very effective, but with time spambot algorithms were
developed to bypass CAPTCHA really well. Now CAPTCHA complexity is not enough
to deter spambots, but is too difficult to the real users who get frustrated
and often just leave the site when confronted with extra burden of CAPTCHA.

In BOTCHA, we don't abuse our human users - BOTCHA protection is completely
transparent to them and non-intrusive.

BOTCHA is useful for any form that has to be protected from spambots.

BOTCHA lets spambots to prove they are bots, and let real users zip by.

Note: BOTCHA does NOT attempt to block human entered spam.

How it works
------------

The approach of BOTCHA is to add various elements to forms that need protection from bots. These elements do not present new fields to users, so BOTCHA is completely transparent to humans. Both humans and bots submit those forms and BOTCHA performs heuristic analysis on each submitted form. Bots are usually programs/scripts that are relatively dumb, and most of the time they fail BOTCHA tests and human users don't.

Once BOTCHA proves the submission is by a bot, the form submission is blocked.

The more there are opportunities for the bot to slip and prove it is a bot, the better defense from spam we have. So we can combine multiple BOTCHA recipes as opposed to only one CAPTCHA per form. This gives huge advantage to BOTCHA.

Advantages
----------

There are many advantages of BOTCHA over CAPTCHA:

* BOTCHA does not bother normal human users
* BOTCHA tests are designed in such a way that normal users will never see them
* There is no limitation on number of tests BOTCHA can implement on each form, so it gets progressively stronger
* As bots get smarter, BOTCHA will be updated with new recipes to defeat them
* BOTCHA needs very little configuration

It is possible to use BOTCHA alone without CAPTCHA. Nevertheless, it is recommended to use BOTCHA together with CAPTCHA. BOTCHA does not interfere with CAPTCHA, and more lines of defense are always better.

What "BOTCHA" means?
--------------------

BOTCHA stands for "BOT Computerized Heuristic Analysis"

BOTCHA also means "Bombs On Target, Come Home Alive" (military, UrbanDictionary).
"BOTCHA is a feel-good cheer after bombing spambots to the ground."

BOTCHA also means "Double-dead meat" (Wikipedia), which is a health hazard.
"We feed BOTCHA to spambots and wait for them to get diarrhea and food poisoning."

Installation
------------

* Copy the module's directory to your sites/all/modules directory
* Activate the module

Usage
-----

Module starts working as soon as it is activated. There are reasonable default settings and no configuration is required, though it can be adjusted at any time on Administration > Configuration > People > BOTCHA page.

Module records its activity in the log and collects statistics which are shown on the 'Status report' page.

There are some default forms that BOTCHA protects out-of-the-box, including user/register, which is the most important line of defense. Current version by default protects all other forms that CAPTCHA is enabled for. CAPTCHA is not required since BOTCHA can be configured independently.

BOTCHA configuration page allows selecting which forms to protect. There is also an admin mode checkbox which adds links to forms for simple BOTCHA configuration.

Please note!: Using BOTCHA logging setting could cause at high levels putting vulnerable data into logs. We have some basic escaping (e.g., for password field) - but any other data could be found in raw format. Please be careful with logging level setting!

Hidden settings
---------------

Hidden settings are variables that you can define by adding them to the $conf
array in your settings.php file.

Name:        botcha_enabled_<form_id>
Default:     FALSE
Description: Set to TRUE for enabling BOTCHA protection for the concrete form.
Example:     variable_set('botcha_enabled_user_register_form', 1)

History
-------

I first developed fully-automated method to protect HTML forms open to un-authenticated users back in 2002, long before I started using Drupal. That method had proven very effective back then. Time went by, and I moved that website to Drupal, and installed CAPTCHA. Eventually the site started getting few dozen automated user registrations a day followed by few spam posts on each account. I tried strengthening Captcha settings, but the stream did not slow down while human users started to complain of the difficulty of Captcha challenges. Apparently Captcha is not deterring recent generation of spambot scripts. I turned back to the old method, and wrote a module for that. The stream stopped and I have zero spambot user registrations since. Now I want to share this module.


