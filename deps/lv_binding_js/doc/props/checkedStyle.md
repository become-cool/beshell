# Component prop checkedStyle

Prop checkedStyle mapping lvgl object LV_STATE_CHECKED style

## Value
support JS Object or JS Array

## Usage
```jsx
import { Switch } from 'lvlgjs-ui'
import { useState } from 'react'

function Component () {
    const [value, setValue] = useState()
    return (
        <React.Fragment>
            {/* controlled */}
            <Switch
              style={style.switch}
              onChange={(e) => setValue(e.checked)}
              checked={value}
              checkedStyle={style.checkedStyle}
            />
            {/* not controlled */}
            <Switch
              style={style.Switch}
              checkedStyle={style.checkedStyle}
              onChange={(e) => console.log(e)}
            />
        </React.Fragment>
    )
}

const style = {
    switch: {},
    checkedStyle: {}
}
```
