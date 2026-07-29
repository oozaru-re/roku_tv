
# Flipper-based Roku TV control

A really ugly controller that does what the standard controller already does, but uses the Flipper because everything becomes much more *Mr. Robot* that way.

## Authors
Me, myself and i
- [@oozaru-re](https://github.com/oozaru-re) 


## FAQ

#### Will this project receive an update?

Nah, probably not. I don't even like this TV, it was just cheap.

#### The icons are terrible, aren't you going to fix them?

Probably not. I said it was ugly.

#### “But I’m on Windows—what’s the command?”

Seems that > you < have a problem

## Installation

First, clone the repository.

```bash
  git clone https://github.com/oozaru-re/roku_tv.git
  cd roku_tv 
```
Then use `pipenv shell`.


Install ufbt inside the env

```bash 
python3 -m pip install --upgrade ufbt
```

Connect your Flipper to the USB port

```bash
ufbt launch
```

The app will open on the Flipper Zero

