#include "task.h"

// ���� ---��������ݽṹ(�����õ��ǽṹ��)�����Եķ�ʽ����

/*��������˳��Ϊ

1.����һ�� �ṹ��ָ��(head_handle) ����ָ��ָ�� ��ַ0

2.�½�һ�� �ṹ�� ��ʼ���������ֵ---����(timer_init)

3.��ԭ�е�ָ��(head_handle) ��ŵ� �½����Ľṹ��(2������)�� �Դ��γ�����--����(timer_start)

    3.1 ��¼��ԭ��ָ��(head_handle)��ֹԭ��ָ�붪ʧ,
        ������������ȷ����ָ��(2������)��ԭ�������е�ָ�벻�ظ� �� ��ԭ��ָ��(head_handle) ����� ��ָ����(2������)
    3.2 ���Ǽ�¼��ָ���ַ(���޸ĺõ�ָ��(2������)��ֵ��(head_handle),�洢,�����´����ʱ������������)
    3.ע ���������Ƿ�����ӵ�,����ӵ��ں�����,����ӵ���ǰ��,

4.�������ָ����������޳� --����(timer_stop)

    4.1 ����һ������ָ�� ���� ��¼�޸ĵ�ַ
    4.2 ѭ������ÿһ��Ԫ��
        4.2.1 �жϵ�ǰԪ���Ƿ�������Ԫ��
            ��  ����ǰԪ��(����Ԫ��)��nextֵ ��ֵ���ϸ�Ԫ�ص�next
            ��  ����ǰԪ�ص�next����ĵ�ַ,��ֵ����ʱ���������������һ��Ԫ��

5.����ִ�м���������  --������timer_loop��

*/

// timer handle list head.
static struct Timer *head_handle = 0; // ����һ����ʼָ��,����ֵ����ֵΪ0

// Timer ticks
static unsigned int _timer_ticks = 0; // ����һ���������ڼ�¼ʱ���ж�����

/**
 * @brief  Initializes the timer struct handle.
 * @param  handle: the timer handle strcut.
 * @param  timeout_cb: timeout callback.
 * @param  repeat: repeat interval time.
 * @retval None
 */
// ����һ���ṹ�崢�涨ʱ���Ľṹ��,����ֵ
void timer_init(struct Timer *handle, void (*timeout_cb)(), unsigned int timeout, unsigned int repeat)
{
    // memset(handle, sizeof(struct Timer), 0);
    // �ص�������ַ(ָ���ַ���Ǻ�������)
    handle->timeout_cb = timeout_cb;

    // �ɼ����ڸ�ֵ(���ʱ��)
    handle->timeout = _timer_ticks + timeout;

    // ����һ����ַ��ʼ��Ϊ��
    handle->next = 0;

    // �Ƿ��ظ�����,ֻ���ظ��Ͳ��ظ�,Ϊ��ֹ�������,��0��Ϊ�ظ�
    if (repeat != 0)
        // �ظ������Ļ��������ʱ�丳ֵ�� repeat  ��  timeout������¼�ϴδ���ʱ��
        handle->repeat = timeout;
    else
        handle->repeat = 0;
}

/**
 * @brief  Start the timer work, add the handle into work list.
 * @param  btn: target handle strcut.
 * @retval 0: succeed. -1: already exist.
 */
// ���½��Ľṹ����ӽ�������  �¼���ı�ָ���ַ
int timer_start(struct Timer *handle)
{
    // ����ʼ����ֵ����ʱֵ
    struct Timer *target = head_handle;

    // ��������ȷ�����������һ��������-֮ǰû�������
    while (target)
    {

        // �������һ��Ԫ�ؿ�ʼ�ȶ� ��κ��¼����һ�����˳����������� -1 ����ȷ��ִ��״̬ (����ȷ��û���ظ�����)
        if (target == handle)
            return -1; // already exist.

        // ȷ����Ԫ�ز������һ����,���¸�һ��Ԫ�ص�ַ��ֵ����ʱֵ
        target = target->next;
    }

    // ���¼��������洢����һ����ַ�޸�Ϊ����ĵ�ַ
    handle->next = head_handle;

    // ���¼���������ַ�洢
    head_handle = handle;

    // �ȶ�ͨ�����Ӻ󷵻�0 ����ȷ��ִ��״̬
    return 0;
}

/**
 * @brief  Stop the timer work, remove the handle off work list.
 * @param  handle: target handle strcut.
 * @retval None
 */
void timer_stop(struct Timer *handle)
{
    // ����һ������ָ��(ָ��ָ���ָ��)
    struct Timer **curr;

    // �ӵ�һ��Ԫ�ؿ�ʼ���� ,���Ҳ��Ԫ��Ϊ0,���鵽����Զ�����ѭ��
    for (curr = &head_handle; *curr;)
    {

        struct Timer *entry = *curr;

        // �жϵ�ǰԪ���ǲ�������Ԫ��(ֱ�ӶԱ�Ԫ�ص��ڴ��ַ)
        if (entry == handle)
        {

            // ����һ���ĵ�ַ���浽��һ����next��

            // curr Ϊ����ָ��ı���(����ָ������������,����������Ϣ) �������һ����Ӧ�ĵ�ַ(�˵�ַ��ѭ����ʼ��ֵ,ֵΪ���һ����ӽ���������ṹ���ַ)
            // *curr Ϊ����ָ������ú��һ��ָ��  ������ú��� �ṹ�屾��,��ָ�뷽ʽ����Ϊ��ǰ�ṹ��ĵ�ַ

            // ��ǰ�ṹ��ĵ�ַ�޸�Ϊ��һ���ṹ��ĵ�ַ(Ӱ��ԭ������)

            // ��ǰ��ֵΪ��������һ��Ԫ�صĵ�ַ ,������֮��, ����next ���´��浽��ǰλ��(��һ��Ԫ�ص�next��)
            *curr = entry->next;
            //          free(entry);
        }
        else
            // �޸���ֱ��ָ������洢��ֵ(��Ӱ��ԭ������)
            curr = &entry->next;
    }
}

/**
 * @brief  main loop.
 * @param  None.
 * @retval None
 */
void timer_loop()
{
    // ������ʱ����
    struct Timer *target;

    // ������������
    for (target = head_handle; target; target = target->next)
    {

        // ��ǰԪ�صĲɼ������Ƿ���ڼ�ʱ����
        if ((int)target->timeout - (int)_timer_ticks <= 0)
        { // To prevent overflow

            // �ж��Ƿ�����ѭ��
            if (target->repeat == 0)
            {
                // ��δ����ѭ����Ԫ���޳�����
                timer_stop(target);
            }
            else
            {
                // ��ʼ��ʱ�� �Ѿ������ʱ�丳ֵ��repeat�ˣ�timeout������¼�ĵ���ʱ��
                // �´ε���ʱ�� =  ��ǰʱ�� + ���ʱ��
                target->timeout = _timer_ticks + target->repeat;
            }
            // ִ�е�ǰԪ�صĻص�����
            target->timeout_cb();
        }
    }
}

/**
 * @brief  background ticks, timer repeat invoking interval 1ms.
 * @param  None.
 * @retval None.
 */
void timer_ticks()
{
    _timer_ticks++;
}
