# SoftwareSecurityTeam03
Software Security Team 03 Team Project - Broker App

## BROKER Application - Build Guide

Install qt5 with below command
```
sudo apt install qt5-default
```

Compile BROKER application with below commands
```
qmake BROKER1.pro
```

```
make
```

Run BROKER application with below command
```
./BROKER1
```

## How to use the application

**Notice** The application works only within the local scope.
Currently it's more like a simulation rather than actual service.
It uses local database.

### 1. Sign up and Login

In order to use the application, you need to sign up firstly.
Login with the created account to proceed to the next step.

### 2. Deposit and Withdrawl

The BROKER app is designed to simulate the anti-fraudalent goods trade
between buyer and seller.

To make a trade, user needs to deposit their money.
It can be simply done by checking the checkbox and clicking the button on the page.

### 3. Transaction Management

By entering to the transaction page from the main window,
You can 1. Request transaction or 2. Load pre-made transaction from web
as **buyer**.
If you added new transaction with those ways,
you will be able to see it from your "to-buy" table.
Seller can also check it from "to-sell" table 
You can proceed the transaction to the next stage(which is represented as status column in the table)
by double-clicking the transaction on the table.

Follow the instructions, and go through all the steps for
the safe trade.


## Website that can use BROKER service

[Secondhand](http://3.36.207.92/)

Sign up and upload your secondhand item for sale.
With another ID you can post the 'buy request'.
If the request is accepted by the seller,
those two buyer and seller will be able to
check each other's BROKER ID.

If those two IDs are registered in your local app,
the buyer can register the information in local DB
by clicking the "load from web" button.

Note that the app itself does not support network
communication to the other app, it's designed to
function within the local scope for thr GITCTF.
