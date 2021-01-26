# Copyright (C) 2015-2020, Wazuh Inc.
# Created by Wazuh, Inc. <info@wazuh.com>.
# This program is free software; you can redistribute it and/or modify it under the terms of GPLv2

from wazuh.core import active_response, common
from wazuh.core.exception import WazuhException
from wazuh.core.ossec_queue import OssecQueue
from wazuh.core.results import AffectedItemsWazuhResult
from wazuh.core.utils import WazuhVersion
from wazuh.rbac.decorators import expose_resources


@expose_resources(actions=['active-response:command'], resources=['agent:id:{agent_list}'])
def run_command(agent_list=None, command=None, arguments=None, custom=False, alert=None):
    """Run AR command in a specific agent
    TODO: change docstrings to numpydoc
    :param agent_list: Run AR command in the agent.
    :param command: Command running in the agent. If this value starts by !, then it refers to a script name instead of
    a command name
    :param custom: Whether the specified command is a custom command or not
    :param arguments: Command arguments
    :return: AffectedItemsWazuhResult.
    """
    oq = OssecQueue(common.ARQUEUE)
    result = AffectedItemsWazuhResult(all_msg='AR command was sent to all agents',
                                      some_msg='AR command was not sent to some agents',
                                      none_msg='AR command was not sent to any agent'
                                      )
    for agent_id in agent_list:
        try:
            # Create classic msg or JSON msg depending on the agent version
            agent_version = agent_id.get_basic_information(select=['version'])['version']
            if WazuhVersion(agent_version) >= WazuhVersion('Wazuh v4.2.0'):
                msg_queue = active_response.create_json_message(command=command, arguments=arguments, alert=alert)
            else:
                msg_queue = active_response.create_message(command=command, arguments=arguments, custom=custom)
            active_response.send_command(msg_queue, oq, agent_id)
            result.affected_items.append(agent_id)
            result.total_affected_items += 1
        except WazuhException as e:
            result.add_failed_item(id_=agent_id, error=e)
    oq.close()

    return result
