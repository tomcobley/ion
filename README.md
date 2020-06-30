# ion
An IoT solution for extending your laptop battery's life.

## Look after your battery
_ion_ is a command line program, used to interact with a smart plug to easily control your battery's charge cycles. Research shows that cycling your batteries charge level between 30% and 75% (rather than 0%-100%) can greatly extend its service life, which is good for your wallet and for the environment.

**Contributions welcomed!** We'd love for you to get involved. Just submit a pull request or open an issue.

## Installation & Setup
You will need a smart plug (any will do, as long as it's supported by [IFTTT](https://ifttt.com/discover/smart-plug-roundup).
(Note: if your smart plug is not supported by IFTTT you may still be able to use our software to activate/deactivate it, but this isn't covered in this guide)).

### Installation
You can install _ion_ wherever you like, but we recommend creating a new directory for it in your home folder. To do so, type the following command into your terminal or command prompt:
```bash
 mkdir ~/.ion
 ```
 Now, navigate into this new directory:
```bash
 cd ~/.ion
 ```
... and install the repository with git (follow [these instructions](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git) to install git if you don't have it):
```bash
 git clone https://github.com/tomcobley/ion
 ```
 This should have created a new directory within your current directory called `ion`. Navigate into it and take a look around with:
```bash
 cd ion
 ls
 ```

#### Dependencies
Depending on your operating system (currently most versions of MacOS and Linux are supported), you may need to execute one of the following commands:
___
##### MacOS
```bash
 brew install curl
 ```
 If you don't have brew already (you should, it's good), follow the instructions [here](https://brew.sh/) to install it.
##### Linux
```bash
 sudo apt-get install libcurl4-gnutls-dev
 ```
___
You have now installed the code and dependencies, and are ready to proceed with the next section.

### Setup

#### Compilation
Make sure you're in the `ion`  directory that was created in the previous step. If you're not, move back there with:
```bash
 cd ~/.ion/ion
 ```
To _compile_ the code, run:
```bash
 make
 ```
Next, run:
```bash
 ./setup.sh
 ```
This will create a _shell script_ called `/usr/local/bin/ion`, which can be used to run the program.

#### IFTTT

Next, you need to create an applet on [IFTTT](https://ifttt.com). To do so, [create an IFTTT account]([https://ifttt.com/join](https://ifttt.com/join)). Once you have done this, click the icon in the top right corner, then click Create to [create an applet](https://ifttt.com/create).

Click on 'This' to add a trigger signal. On the 'Choose a service' page, search for and select the service called 'Webhooks', then on the 'Choose trigger' page, click 'Receive a web request'. Write a name for the event (we recommend "**charge_low**") and select Create trigger.

Now click on the 'That' button to choose an action. Search for the brand of smart plug that you have, and select the relevant option. If you haven't already set up your smart plug, you should do that now, following the manufacturer's instructions. You may have to log in to your account with the smart plug manufacturer to link your smart plug. It is important that you link this "charge_low" action with the **turning on** of the smart plug. This action will be triggered when the charge of your laptop becomes low, turning on your laptop charger.

Repeat the above process, creating a new applet, this time with the event name "**charge_high**", and the action being the **turning off** of the same smart plug. This action will be triggered when the charge of your laptop becomes high, turning off your laptop charger.

Now, find out your *maker webhook key*, by visiting [this page](https://ifttt.com/maker_webhooks/settings). Under Account Info, you will see your URL, something like `https://maker.ifttt.com/use/xxxxxxxxxxx-xxxxxxxxxx`, where the section following the last `/`  is your unique *webhook key*. Save this key for later.

#### Configuring _ion_

To configure your settings, run:
```bash
 ion init
 ```
You will now be able to enter the URLs used to trigger your IFTTT applets:
___
Your first response should as follows, but with `EVENT_NAME` replaced by `charge_low` (or whatever name you used when creating your first webhook), and `xxxxxxxxxxx-xxxxxxxxxx` replaced by your *webhook key*:
```
 https://maker.ifttt.com/trigger/EVENT_NAME/with/key/xxxxxxxxxxx-xxxxxxxxxx
```
Your second response should be the same, but with `EVENT_NAME` replaced by `charge_high` (or whatever name you used when creating your second webhook).

For the charge lower and upper bounds, we recommend you use the provided system defaults.
___

 You should now be able to manually run the program, give it a try with:
```bash
 ion
 ```
 You should also be able to run the test suite to check everything has been set up correctly. Try it with:
```bash
 cd tests
 make runtests
```
It is specifically important that the webhook tests pass. Moreover, make sure that the plug turns ON and then OFF when the test is running. Otherwise, you may have configured your IFTTT tasks incorrectly.

Also, feel free to test if the program is interfacing with the smart plug correctly by adjusting the lower and upper bounds so your current battery percentage is outside the range you specified.

#### Cron
Keep following the steps to get the program to run automatically rather than manually.

We need to add a _cron job_, which will run the program once a minute (important to keep an eye on your battery level). Run:
```bash
 crontab -e
 ```
You may get a message asking you to pick an editor. Pick your favourite, or Nano (the easiest). Now add the following line to the file and exit the editor:
  ```bash
* * * * * /usr/local/bin/ion
 ```

If you're on a Mac, you may get a popup saying `Terminal would like to administer your computer`, click OK.

This will run the shell script we created earlier (which runs _ion_) once per minute.

 ___
That's it! The program should now keep your laptop battery within the range you specified, keeping your battery healthy for longer. If you have any problems, please submit an issue on GitHub.

## Authors

_ion_ was developed by Tom Cobley, Andrew Elkadi, Dimitri Chamay and Nicholas Duer.
