local targetPlatform = cc.Application:getInstance():getTargetPlatform()

local LINE_SPACE = 40
local itemTagBasic = 1000
local LuaBridgeTestsName =
{
    "LuaJavaBridge",
    "LuaObjectCBridge",
    "LuaTableToObjcDictTest",
}

local s = cc.Director:getInstance():getWinSize()

local function LuaBridgeLayer()
    local layer = cc.Layer:create()
    local menu = cc.Menu:create()
    menu:setPosition(cc.p(0, 0))
    cc.MenuItemFont:setFontName("Arial")
    cc.MenuItemFont:setFontSize(24)

    local supportObjectCBridge  = false
    if (cc.PLATFORM_OS_IPHONE == targetPlatform) or (cc.PLATFORM_OS_IPAD == targetPlatform) or (cc.PLATFORM_OS_MAC == targetPlatform)  then
        supportObjectCBridge = true
    end

    local supportJavaBridge = false
    if (cc.PLATFORM_OS_ANDROID == targetPlatform) then
        supportJavaBridge = true
    end

    local function createMenuToBridgeScene()
        local menuToBridgeScene = cc.Menu:create()
        local function toBridgeScene()
            local scene = LuaBridgeMainTest()
            if scene ~= nil then
                cc.Director:getInstance():replaceScene(scene)
            end
        end 
        --Create BackMneu
        cc.MenuItemFont:setFontName("Arial")
        cc.MenuItemFont:setFontSize(24)
        local menuItemFont = cc.MenuItemFont:create("Back")
        menuItemFont:setPosition(cc.p(VisibleRect:rightBottom().x - 50, VisibleRect:rightBottom().y + 25))
        menuItemFont:registerScriptTapHandler(toBridgeScene)
        menuToBridgeScene:addChild(menuItemFont)        
        menuToBridgeScene:setPosition(cc.p(0, 0))
        return menuToBridgeScene
    end

    local function newLuaJavaBridge()
        local newScene   = cc.Scene:create()
        local titleLabel = cc.Label:createWithTTF("", s_arialPath, 28)
        newScene:addChild(titleLabel, 1)
        titleLabel:setAnchorPoint(cc.p(0.5, 0.5))
        titleLabel:setPosition(s.width / 2, s.height - 50)
        titleLabel:setString("LuaJavaBridge Test")

        subtitleLabel = cc.Label:createWithTTF("", s_thonburiPath, 16)
        newScene:addChild(subtitleLabel, 1)
        subtitleLabel:setAnchorPoint(cc.p(0.5, 0.5))
        subtitleLabel:setPosition(s.width / 2, s.height - 80)
        subtitleLabel:setString("See the console.")
        if (cc.PLATFORM_OS_ANDROID == targetPlatform) then
            local args = { 2 , 3}
            local sigs = "(II)I"
            local luaj = require "axmol.cocos2d.luaj"
            local className = "com/cocos2dx/sample/LuaJavaBridgeTest/LuaJavaBridgeTest"
            local ok,ret  = luaj.callStaticMethod(className,"addTwoNumbers",args,sigs)
            if not ok then
                print("luaj error:", ret)
            else
                print("The ret is:", ret)
            end

            local function callbackLua(param)
                if "success" == param then
                    print("java call back success")
                end
            end
            args = { "callbacklua", callbackLua }
            sigs = "(Ljava/lang/String;I)V"
            ok = luaj.callStaticMethod(className,"callbackLua",args,sigs)
            if not ok then
                print("call callback error")
            end
        end
        return newScene
    end

    local function newLuaObjectCBridge()
        local newScene = cc.Scene:create()
        local titleLabel = cc.Label:createWithTTF("", s_arialPath, 28)
        newScene:addChild(titleLabel, 1)
        titleLabel:setAnchorPoint(cc.p(0.5, 0.5))
        titleLabel:setPosition(s.width / 2, s.height - 50)
        titleLabel:setString("LuaObjectCBridge Test")

        subtitleLabel = cc.Label:createWithTTF("", s_thonburiPath, 16)
        newScene:addChild(subtitleLabel, 1)
        subtitleLabel:setAnchorPoint(cc.p(0.5, 0.5))
        subtitleLabel:setPosition(s.width / 2, s.height - 80)
        subtitleLabel:setString("See the console.")
        if (cc.PLATFORM_OS_IPHONE == targetPlatform) or (cc.PLATFORM_OS_IPAD == targetPlatform) or (cc.PLATFORM_OS_MAC == targetPlatform) then
            local args = { num1 = 2 , num2 = 3 }
            local luaoc = require "axmol.cocos2d.luaoc"
            local className = "LuaObjectCBridgeTest"
            local ok,ret  = luaoc.callStaticMethod(className,"addTwoNumbers",args)
            if not ok then
                Director:getInstance():resume()
            else
                print("The ret is:", ret)
            end

            local function callback(param)
                if "success" == param then
                    print("object c call back success")
                end
            end
            luaoc.callStaticMethod(className,"registerScriptHandler", {scriptHandler = callback } )
            luaoc.callStaticMethod(className,"callbackScriptHandler")
        end
        return newScene
    end

    local function newLuaTableToObjcDict()
        local newScene = cc.Scene:create()
        local titleLabel = cc.Label:createWithTTF("", s_arialPath, 28)
        newScene:addChild(titleLabel, 1)
        titleLabel:setAnchorPoint(cc.p(0.5, 0.5))
        titleLabel:setPosition(s.width / 2, s.height - 50)
        titleLabel:setString("LuaTableToObjcDict Test")

        subtitleLabel = cc.Label:createWithTTF("", s_thonburiPath, 16)
        newScene:addChild(subtitleLabel, 1)
        subtitleLabel:setAnchorPoint(cc.p(0.5, 0.5))
        subtitleLabel:setPosition(s.width / 2, s.height - 80)
        subtitleLabel:setString("See the console.")
        if (cc.PLATFORM_OS_IPHONE == targetPlatform) or (cc.PLATFORM_OS_IPAD == targetPlatform) or (cc.PLATFORM_OS_MAC == targetPlatform) then
            local args = {name = "jett" ,method = "call OC"}
            local args_tab = {tab = args , name = "table_test"}
            local luaoc = require "axmol.cocos2d.luaoc"
            local className = "LuaObjectCBridgeTest"
            local ok,ret  = luaoc.callStaticMethod(className,"printLuaTableToDictionary",args_tab)

        end
        return newScene
    end


    local function newLuaBridgeScene(idx)
        if 1 == idx then
            return newLuaJavaBridge()
        elseif 2 == idx then
            return newLuaObjectCBridge()
        elseif 3 == idx then
            return newLuaTableToObjcDict()
        end
    end

    local function menuCallback(tag, pMenuItem)
        local scene = nil
        local nIdx = pMenuItem:getLocalZOrder() - itemTagBasic
        local newScene = newLuaBridgeScene(nIdx)
        if nil ~= newScene then
            newScene:addChild(createMenuToBridgeScene(),10)
            cc.Director:getInstance():replaceScene(newScene)
        end
    end

    for i = 1, #(LuaBridgeTestsName) do
        local item = cc.MenuItemFont:create(LuaBridgeTestsName[i])
        item:registerScriptTapHandler(menuCallback)
        item:setPosition(s.width / 2, s.height - i * LINE_SPACE)
        menu:addChild(item, itemTagBasic + i)
        if ((i == 1) and (false == supportJavaBridge))
        or ((i == 2) and (false == supportObjectCBridge)) 
        or ((i == 3) and (false == supportObjectCBridge))then
            item:setEnabled(false)
        end
    end

    layer:addChild(menu)

    return layer
end

-------------------------------------
--  LuaBridge Test
-------------------------------------
function LuaBridgeMainTest()
    local scene = cc.Scene:create()
    scene:addChild(LuaBridgeLayer())
    scene:addChild(CreateBackMenuItem())
    return scene
end
