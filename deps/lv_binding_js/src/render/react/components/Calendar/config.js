import { CalendarComp } from './comp'

export default class CalendarConfig {
    tagName = 'Calendar';
    native = null;
    shouldSetTextContent() {
        return false;
    }
    createInstance(
        newProps,
        rootInstance,
        context,
        workInProgress,
        uid
    ) {
        const instance = new CalendarComp({ uid });
        instance.setProps(newProps, {});
        return instance;
    }
    commitMount(
        instance,
        newProps,
        internalInstanceHandle
    ) {
    }
    commitUpdate(
        instance,
        updatePayload,
        oldProps,
        newProps,
        finishedWork
    ) {
        instance.setProps(newProps, oldProps);
    }
    setProps(newProps, oldProps) {
    }
    insertBefore(child, beforeChild) {

    }
    appendInitialChild(child) {
    }
    appendChild(child) {
        
    }
    removeChild(child) {
    }
}
