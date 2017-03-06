import time
import forecastio
from beautifulhue.api import Bridge

# For Hue API
username = '9abbkhNa3vKMJh89BuVodUPSVHcrAMAyEPsrZFnK' # set from the api in Getting-Started
bridge = Bridge(device={'ip':'10.0.0.143'}, user={'name':username})


# For DarkSky Api
# https://api.darksky.net/forecast/ba0895c5ed244c70b224a660049f9555/40.00045466,-104.98455273
api_key = "ba0895c5ed244c70b224a660049f9555"
lat = 40.00045466
lng = -104.98455273

def getCurrentTemp():
    forecast = forecastio.load_forecast(api_key, lat, lng)
    byHour = forecast.hourly()
    return forecast.currently().temperature

def changeCt(light,ct):
    resource = {
        'which':light,
        'data':{
            'state':{'on':True, 'ct':ct}
        },
        'verbose': True
    }
    bridge.light.update(resource)

while(True):
    temp = getCurrentTemp()
    adjTemp = 153+temp*5
    if(adjTemp > 500):
        adjTemp = 500
    elif(adjTemp < 153):
        adjTemp = 153
    adjTemp = int(adjTemp)
    changeCt(3,adjTemp)
    print temp, adjTemp
    time.sleep(60*4)
